/* @(#)Z 1.41 com/src/registry/ODPrtRei.cpp, odconfig, od96os2, odos29646d 96/11/15 15:49:16 (96/11/07 11:56:02) */

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
/*  IBM Change History (most recent first):                         */
/*  143887   11/28/95   jso  Create only one template registering.  */
/*  143600   11/14/95   jso  fixed up error codes.               .  */
/*  143601   11/13/95   pfe  change ODRemoveTemp. to ODDeleteTemp.  */
/*  142016   11/06/95   jso  Commented out somLPrintf calls         */
/*  142527   11/01/95   jso  Fixed somUninit                        */
/*  141034   10/24/95   jso  Check for NULL in GetPartHandlerInfo   */
/*  124840   10/04/95   jso  Added Bento Semaphore protection       */
/*  128523   09/28/95   jso  Added File Category preferences        */
/*           09/08/95   mjk  Add Unicode Support                    */
/*  129803   07/20/95   jso  GetPartHandlerInfo returns Null if     */
/*                           not found                              */
/*  126381   07/18/95   jso  Added Exception Handling to Register   */
/*  123473   07/14/95   jso  Added FileType/Ext preferences         */
/*  128470   07/07/95   jso  Added ODDebug ifdef for somLPrintf     */
/*  123479   06/28/95   jso  Added UpdateODPartHandlerInfo          */
/*  127295   06/27/95   pfe  change to templates call.              */
/*  123475   06/20/95   jso  Fix for Deregister Method.             */
/*  123485   06/07/95   jso  Added Remove Template call.            */
/*  121007   05/31/95   jso  Added Template flag.                   */
/*  122710  5/18/95 ced  Create shell plug-in folder if needed.     */
/********************************************************************/

// law, 24332, 8/2/96: had to add this define in order to compile
// with the new Boolean defines in ODTypesP.h
#ifdef _PLATFORM_AIX_
#define __TYPES__   
#endif
#define ODPartHandlerRegistryInt_Class_Source
#define VARIABLE_MACROS
#include <ODPrtRei.xih>
#include <PartInfi.xh>
#include <PartKind.xh>
#include <PartCat.xh>

#ifdef _PLATFORM_OS2_
#define INCL_DOSSEMAPHORES
#define INCL_DOSERRORS
#define INCL_DOSMISC
#define INCL_WINWORKPLACE

#ifdef _UNICODE_
#define INCL_WIN
#endif // _UNICODE_

#include <OS2.H>
#include <ODTemps.h>
#endif // _PLATFORM_OS2_

#include <iset.h>
#include <csetwrap.h>

#include <Part.xh>
#include <rmutil.h>
#include <rmprptyp.h>
#include <sys/stat.h>

#if defined (_PLATFORM_OS2_) || defined (_PLATFORM_WIN32_)
#include <direct.h>
#endif
#ifdef _PLATFORM_OS2_
#include <io.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>

#ifdef _PLATFORM_WIN32_
#include "Windreg.h"
#endif

static Node* opendocNode = 0;
static Node* partHandlersNode = 0;
static Node* partKindNode = 0;
static Node* partCategoryNode = 0;

#ifndef _SESSHDR_
#include <SessHdr.h>            // for sessionRoutinesMetahandler
#endif

#ifndef _FSHDR_
#include <FSHdr.h>
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif

#include "xhandlrs.h"
#include "xembeded.h"
#include "xsession.h"
#include "TOCEnts.h"
#include "Containr.h"

//*** Constants      ******************************************************
const  kMaxBufferSize             = 1024;
const  ODISOStr ODWorkFileName          = "\\work";

char *ODFileNames[] = {"\\work\\statry","\\work\\plugins","\\work\\editors","\\work\\docs"};
char *ODFolderId[] = {"<OD_STATRY>", "<OD_PLUGINS>", "<OD_EDITORS>", "<OD_DOCS>"};

#if WANT_REGISTRY
void Initialize()
{
    ns_error_t rc;

    Path odPath( 1, "OPENDOC");

    if ( Node::getRootNode(). resolve( odPath, &opendocNode) != NS_SUCCESS)
    {
        opendocNode = new Node( 1, NS_NODE_TYPE_DIR);
        opendocNode-> setAttribute( 0, "NAME", "OpenDoc");

        if ( (rc = Node::getRootNode(). addNode( odPath, opendocNode)) != NS_SUCCESS )
        {
            #if ODDebug
            //PRINT( "Failed to create OpenDoc Node.  RC = %0x\n", rc);
            #endif
            return;
        }
    }

    Path partPath( 1, "PARTHANDLER");

    if ( opendocNode-> resolve( partPath, &partHandlersNode) != NS_SUCCESS)
    {
        partHandlersNode = new Node( 2, NS_NODE_TYPE_DIR);
        partHandlersNode-> setAttribute( 0, "NAME", "PartHandler");
        partHandlersNode-> setAttribute( 1, "COUNT", 0UL);

        if ( (rc = opendocNode-> addNode( partPath, partHandlersNode)) != NS_SUCCESS )
        {
            #if ODDebug
            //PRINT( "Failed to create PartHandler Node.  RC = %0x\n", rc);
            #endif
            return;
        }
    }

}
#endif

int StringCompare (const void *arg1, const void *arg2)
{
               /* Compare all of both strings */
   return(strcmp(*(char **)arg1, *(char **)arg2));
}

int CountFileTypes(_IDL_SEQUENCE_PartKindInfo partKinds)
{
  int count = 0;

  for(int i = 0; i < partKinds. _length; i++)
  {
    if(strlen(partKinds. _buffer[i].filenameTypes))
    {
      count++;
      string pWorkArea = newString(partKinds. _buffer[i].filenameTypes);
      char * p = pWorkArea;
      while(*p != '\0')
      {
         if(*p == ',')
            count++;
         p++;
      }
      SOMFree(pWorkArea);
    }
  }

  return count;
}

int CountFileExts(_IDL_SEQUENCE_PartKindInfo partKinds)
{
  int count = 0;

  for(int i = 0; i < partKinds. _length; i++)
  {
    if(strlen(partKinds. _buffer[i].filenameFilters))
    {
      count++;
      string pWorkArea = newString(partKinds. _buffer[i].filenameFilters);
      char * p = pWorkArea;
      while(*p != '\0')
      {
         if(*p == ',')
            count++;
         p++;
      }
      SOMFree(pWorkArea);
    }
  }

  return count;
}

int CountCategories(_IDL_SEQUENCE_PartKindInfo partKinds)
{
  int count = 0;

  for(int i = 0; i < partKinds. _length; i++)
  {
    if(strlen(partKinds. _buffer[i].categories))
    {
      count++;
      string pWorkArea = newString(partKinds. _buffer[i].categories);
      char * p = pWorkArea;
      while(*p != '\0')
      {
         if(*p == ',')
            count++;
         p++;
      }
      SOMFree(pWorkArea);
    }
  }

  return count;
}
_IDL_SEQUENCE_string ParseFileTypeString( PartKindInfo partKinds)
{
  _IDL_SEQUENCE_string types;
  types. _maximum = types. _length  = 0;
  types. _buffer  = 0;
  int count       = 0;

  if(strlen(partKinds.filenameTypes))
  {
    count = 1;
    string pWorkArea = newString(partKinds.filenameTypes);
    char * p = pWorkArea;
    while(*p != '\0')
    {
       if(*p == ',')
          count++;
       p++;
    }
    types. _buffer = (string*)SOMMalloc( count * sizeof( string*) );

    p = strtok(pWorkArea, ",");
    int i = 0;
    if(p)
    {
        types. _buffer[i++] = newString(p);
    }
    else
    {
        types. _buffer[i++] = pWorkArea;
    }
    if(p)
    {
        while( p = strtok(NULL, ",") )
        {
        types. _buffer[i++] = newString(p);
        }
    }

    SOMFree(pWorkArea);
    types. _maximum = types. _length = count;
  }
    return types;
}

_IDL_SEQUENCE_string ParseFileExtString( PartKindInfo partKinds)
{

  _IDL_SEQUENCE_string types;
  types. _maximum = types. _length  = 0;
  types. _buffer  = 0;
  int count       = 0;

  if(strlen(partKinds.filenameFilters))
  {
    count = 1;
    string pWorkArea = newString(partKinds.filenameFilters);
    char * p = pWorkArea;
    while(*p != '\0')
    {
       if(*p == ',')
          count++;
       p++;
    }
    types. _buffer = (string*)SOMMalloc( count * sizeof( string*) );

    p = strtok(pWorkArea, ",");
    int i = 0;
    if(p)
    {
        types. _buffer[i++] = newString(p);
    }
    else
    {
        types. _buffer[i++] = pWorkArea;
    }
    if(p)
    {
        while( p = strtok(NULL, ",") )
        {
        types. _buffer[i++] = newString(p);
        }
    }

    SOMFree(pWorkArea);
    types. _maximum = types. _length = count;
  }
    return types;
}

_IDL_SEQUENCE_string ParseCategoryString( PartKindInfo partKinds)
{
  _IDL_SEQUENCE_string types;
  types. _maximum = types. _length  = 0;
  types. _buffer  = 0;
  int count       = 0;

  if(strlen(partKinds.categories))
  {
    count = 1;
    string pWorkArea = newString(partKinds.categories);
    char * p = pWorkArea;
    while(*p != '\0')
    {
       if(*p == ',')
          count++;
       p++;
    }
    types. _buffer = (string*)SOMMalloc( count * sizeof( string*) );

    p = strtok(pWorkArea, ",");
    int i = 0;
    if(p)
    {
        types. _buffer[i++] = newString(p);
    }
    else
    {
        types. _buffer[i++] = pWorkArea;
    }
    if(p)
    {
        while( p = strtok(NULL, ",") )
        {
        types. _buffer[i++] = newString(p);
        }
    }

    SOMFree(pWorkArea);
    types. _maximum = types. _length = count;
  }
    return types;
}

SOM_Scope void  SOMLINK ODPartHandlerRegistryIntsomUninit(ODPartHandlerRegistryInt *somSelf)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntsomUninit");

    ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_somUninit(somSelf);

    CMEndSession( somThis->session, kODTrue );
    delete somThis->fHandlers;
    delete somThis->fPlatformFile;

    ODPartHandlerInfoInt*     removedPart;
    ODPartKind *              removedKind;
    ODPartFileType *          removedFileType;
    ODPartFileExtension *     removedFileExt;


    ICursor* cursor = somThis-> parts-> newCursor();
    for(cursor->setToFirst(); cursor->isValid(); cursor->setToNext())
    {
       removedPart = (ODPartHandlerInfoInt *)somThis-> parts-> elementAt( *cursor);
       delete removedPart;
    }
    delete somThis-> parts;

    cursor = somThis-> kinds-> newCursor();
    for(cursor->setToFirst(); cursor->isValid(); cursor->setToNext())
    {
       removedKind = (ODPartKind *)somThis-> kinds-> elementAt( *cursor);
       delete removedKind;
    }
    delete somThis-> kinds;

    cursor = somThis-> types-> newCursor();
    for(cursor->setToFirst(); cursor->isValid(); cursor->setToNext())
    {
       removedFileType = (ODPartFileType *)somThis-> types-> elementAt( *cursor);
       delete removedFileType;
    }
    delete somThis-> types;

    cursor = somThis-> extensions-> newCursor();
    for(cursor->setToFirst(); cursor->isValid(); cursor->setToNext())
    {
       removedFileExt = (ODPartFileExtension *)somThis-> extensions-> elementAt( *cursor);
       delete removedFileExt;
    }
    delete somThis-> extensions;

    cursor = somThis-> categories-> newCursor();
    for(cursor->setToFirst(); cursor->isValid(); cursor->setToNext())
    {
       removedKind = (ODPartKind *)somThis-> categories-> elementAt( *cursor);
       delete removedKind;
    }
    delete somThis-> categories;
    delete cursor;
    delete somThis-> completeFileName;

#ifdef _PLATFORM_OS2
    /* 124840 */
    DosCloseMutexSem(somThis->fHMTXBentoAccess);
#endif
}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntRegisterPartHandlerClass(ODPartHandlerRegistryInt *somSelf,
                                                  Environment *ev,
                                                 string className,
                                                 string DLLName,
                                                 long cTemplate,
                                                 long reserved)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","RegisterPartHandlerClass");

    //*** Local varibles ******************************************************
    CMContainer               container;
    ODPartHandlerInfoInt*     partInfo;
    ODPartHandlerInfoInt*     removedPart;
    static ODPartKind*        testKind  = new ODPartKind;
    ODPartKind*               foundKind;
    ODPartKind*               newKind;
    long                      rc;
    _IDL_SEQUENCE_PartKindInfo partKinds;
#ifdef _PLATFORM_OS2_
    HMTX                      hmtx = 0;
#endif
#ifdef _UNICODE_
    char szMsg1[Str255];
    char szMsg2[Str255];
#endif
    //*************************************************************************

    partInfo = new ODPartHandlerInfoInt;


#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    //## 124840 Get the Bento mutex sem to have access to partreg.bto
    if( DosOpenMutexSem(BENTOSEMNAME, &hmtx))
       THROW(kODErrSemaphoreError);

    if(DosRequestMutexSem(hmtx, SEM_INDEFINITE_WAIT))
       THROW(kODErrSemaphoreError);
#endif
#endif
    try {

    container = somSelf-> OpenContainer( ev, CM_READWRITE);

    CMObject partObject = CMNewObject( container);
    rc = partInfo -> InitODPartHandlerInfo( ev, className, DLLName, OS2);


    if ( rc == PR_NO_ERROR )
    {
        partInfo -> WriteODPartHandlerInfoIntToStorage( ev,  partObject);

        #if WANT_REGISTRY
        partInfo -> WriteODPartHandlerInfoIntToRegistry( ev,  partHandlersNode);
        #endif
        //## Just borrow the partKindInfo sequence to initialize the ODPartKind objects.
        partKinds = partInfo-> _get_partKindInfo( ev);

        for ( int i = 0; i < partKinds. _length; i++)
        {
            //## This part cheats a little bit, because a new copy of the partKindName is
            //## is not made just for the test.  If this partKindName is not found then a
            //## new ODPartKind is created which actually has a copy of the partKindName.

            ICursor* cursor = somThis-> kinds-> newCursor();
            CWrap<ODPartKind> kindWrap( testKind);
            testKind-> _set_partKindName( ev, partKinds. _buffer[i]. partKindName);

            if ( somThis-> kinds-> locate( kindWrap, *cursor) )
            {
                foundKind = somThis-> kinds-> elementAt( *cursor);
                foundKind-> Add( ev, partInfo);
            }
            else
            {
                newKind = new ODPartKind;
                newKind-> _set_partKindName( ev, newString( partKinds. _buffer[i]. partKindName));
                somThis-> kinds-> add( newKind);
                newKind-> Add( ev, partInfo);
            }
            delete cursor;
        }
        ICursor* cursor = somThis-> parts-> newCursor();
        CWrap<ODPartHandlerInfoInt> partWrap( partInfo);

        if ( !somThis-> parts-> add( partWrap, *cursor ) )
        {
            removedPart = somThis-> parts-> elementAt( *cursor);
            somThis-> parts-> removeAt( *cursor);
            SOM_Assert( somThis-> parts-> add( partWrap, *cursor ), 1);

            somSelf-> RemoveODPartHandlerInfoFromStorage( ev, removedPart, container);
//            delete removedPart;

            rc = errPR_PART_HANDLER_INFO_REPLACED;
        }
        delete cursor;
    }
    else
    {
        CMDeleteObject( partObject);
        delete partInfo;
    }

    somSelf-> CloseContainer( ev);

#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    if(DosReleaseMutexSem(hmtx)) // 124840
       THROW(kODErrSemaphoreError);
#endif
#endif

#ifdef _PLATFORM_WIN32_
    // Set up the Windows Side
    if(rc == PR_NO_ERROR)
        WindowsRegistry::RegisterPart( partKinds. _buffer[0]. partKindName, partInfo-> _get_partHandlerDisplayName(ev),
            partInfo-> _get_ole2ClassId(ev), partInfo-> _get_windowsIconFilename(ev));
#endif

    if( (rc == PR_NO_ERROR || rc == errPR_PART_HANDLER_INFO_REPLACED) && cTemplate == TRUE)
    {

        ODEditor partEditor = (char *)malloc(strlen(className)+1); //127295 [pfe]
        strcpy(partEditor,className);
       _IDL_SEQUENCE_PartKindInfo partKinds = partInfo-> _get_partKindInfo( ev);

#if 1 // defect 22796
#ifdef _PLATFORM_OS2_
        if( partKinds. _length) // [143887]
        {
             ODCreateTemplate(partKinds. _buffer[0]. partKindName,partEditor);  //127295 [pfe]
        }
#endif
#endif
        free(partEditor); //127295 [pfe]

    }
  } // try
  catch (ODException _exception) {                    // jso [126381]
#ifdef _UNICODE_
     WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                    OD_OPENDOCEXCEPTION, sizeof(szMsg1), szMsg1);
     char fmtstring[Str255];
     strcpy(fmtstring, szMsg1);
     int bufsize = strlen(fmtstring) + strlen(ErrorMessage()) + sizeof(__FUNCTION__) + 11;
#else
     static const char fmtstring[] = "OpenDoc Exception %d caught in %s\n\n%s";
     int bufsize = sizeof(fmtstring) + strlen(ErrorMessage()) + sizeof(__FUNCTION__) + 11;
#endif
     char* buffer = new char[bufsize];
     if (buffer)
        sprintf(buffer, fmtstring, ErrorCode(), __FUNCTION__, ErrorMessage());
#ifdef _UNICODE_
     WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                    OD_OUTOFMEMORY, sizeof(szMsg1), szMsg1);
     WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                    OD_EXCEPTIONS, sizeof(szMsg2), szMsg2);
     buffer ? buffer : szMsg1;
     //somLPrintf( 1, "Opendoc exceptions %s", buffer);
#if Debug
     //PRINT("OpenDoc exception: %s %s\n",szMsg2,buffer);
#endif
     delete[] buffer;
#else
     buffer ? buffer : "Out of memory!!!";
     //somLPrintf( 1, "Opendoc exceptions %s", buffer);
#if Debug
     //PRINT("OpenDoc exception: %s \n",buffer);
#endif
     delete[] buffer;
#endif
  SetErrorCode(kODNoError);
  }
   return rc;
}


SOM_Scope _IDL_SEQUENCE_ISOString  SOMLINK ODPartHandlerRegistryIntGetPartKindList(ODPartHandlerRegistryInt *somSelf,
                                                      Environment *ev,
                                                      ISOString partHandlerName,
                                                      string category)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","GetPartKindList");

    //*** Local varibles ******************************************************
    _IDL_SEQUENCE_ISOString             partKinds;
    _IDL_SEQUENCE_PartKindInfo          partKindInfo;
    ICursor*                            cursor;
    ODPartHandlerInfoInt*               foundPart;
    static ODPartHandlerInfoInt*        testPart  = new ODPartHandlerInfoInt;
    CWrap<ODPartHandlerInfoInt>         partWrap( testPart);
    //*************************************************************************

    try {
    somSelf->CheckAndRefresh(ev);
    partKinds. _length = partKinds. _maximum = 0;
    partKinds. _buffer = 0;

    if ( partHandlerName)
    {
        testPart-> _set_partHandlerName( ev, partHandlerName);

        cursor = somThis-> parts-> newCursor();

        if ( somThis-> parts-> locate( partWrap, *cursor) )
        {
            foundPart = somThis-> parts-> elementAt( *cursor);
            partKindInfo = foundPart-> _get_partKindInfo( ev);

            partKinds. _maximum = partKindInfo. _length;
            partKinds. _length  = partKindInfo. _length;
            if ( partKinds. _length )
            {
                partKinds. _buffer  = (string*)SOMMalloc( partKinds. _length * sizeof( string*) );

                long j = 0;
                for ( long i = 0; i < partKindInfo. _length; i++)
                {
                    if ( category && partKindInfo. _buffer[i]. categories )
                    {
                        if( strstr( partKindInfo. _buffer[i]. categories, category) )
                        {
                            partKinds. _buffer[j++] = newString( partKindInfo. _buffer[i]. partKindName);
                        }
                    }
                    else
                    {
                        partKinds. _buffer[j++] = newString( partKindInfo. _buffer[i]. partKindName);
                    }
                }
                partKinds. _length  = j;
            }
        }
        delete cursor;
    }
    else
    {

        partKinds. _maximum = partKinds. _length = somThis-> kinds-> numberOfElements();

        if ( partKinds. _length)
        {
            partKinds. _buffer  = (string*)SOMMalloc( partKinds. _length * sizeof( string*) );

            long i = 0;
            cursor = somThis-> kinds-> newCursor();

            forCursor( *cursor)
            {
                ODPartKind* kind;
                SOM_Assert( int(kind = somThis-> kinds-> elementAt( *cursor)), 1);

                partKinds. _buffer[i++] = newString( kind-> _get_partKindName( ev));
            }

            delete cursor;
            partKinds. _length = i;

        }
    }
  } // try
  catch (ODException _exception) { 
    SetErrorCode(kODNoError);
  }
   return partKinds;
}

SOM_Scope ODPartHandlerInfo*  SOMLINK ODPartHandlerRegistryIntGetPartHandlerInfo(ODPartHandlerRegistryInt *somSelf,
                                                   Environment *ev,
                                                   ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","GetPartHandlerInfo");

    //*** Local varibles ******************************************************
    static ODPartHandlerInfoInt*        testPart  = new ODPartHandlerInfoInt;
    ODPartHandlerInfoInt*               foundPart = NULL;  /* [129803] */
    ICursor*                            cursor;
    CWrap<ODPartHandlerInfoInt>         partWrap( testPart);
    //*************************************************************************

    if(partHandlerName == NULL) // 141034
       return foundPart;

    try {
    somSelf->CheckAndRefresh(ev);
    cursor = somThis-> parts-> newCursor();

    testPart-> _set_partHandlerName( ev, partHandlerName);

    if ( somThis-> parts-> locate( partWrap, *cursor) )
    {
        foundPart = somThis-> parts-> elementAt( *cursor);
        foundPart = foundPart-> Clone( ev);
    }
    delete cursor;

  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }
   return foundPart;
}


SOM_Scope void  SOMLINK ODPartHandlerRegistryIntInitODPartHandlerRegistryIntWithStorage(ODPartHandlerRegistryInt *somSelf,
                                                       Environment *ev)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntInitODPartHandlerRegistryIntWithStorage");

    //*** Local varibles ******************************************************
    CMObject                      currObject = 0;
    CMContainer                   container;
    ODPartHandlerInfoInt*         partInfo;
    ODPartHandlerInfoInt*         removedPart;
    static ODPartKind*            testKind  = new ODPartKind;
    //_ftestKind  = new ODPartKind;
    ODPartKind*                   newKind;
    ODPartKind*                   foundKind;
    static ODPartFileType*        testType  = new ODPartFileType;
    ODPartFileType*               newType;
    ODPartFileType*               foundType;
    static ODPartFileExtension*   testExtension  = new ODPartFileExtension;
    ODPartFileExtension*          newExtension;
    ODPartFileExtension*          foundExtension;
#ifdef _PLATFORM_OS2_
    HMTX                          hmtx = 0;
#endif
    //*************************************************************************

#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    //## 124840 Get the Bento mutex sem to have access to partreg.bto
    if( DosOpenMutexSem(BENTOSEMNAME, &hmtx))
       THROW(kODErrSemaphoreError);

    if(DosRequestMutexSem(hmtx, SEM_INDEFINITE_WAIT))
       THROW(kODErrSemaphoreError);
#endif
#endif

//    somThis->fPlatformFile = new PlatformFile;
//    somThis->fPlatformFile->Specify((ODFileSpec*) completeFileName);

    try {
    somThis->session = CMStartSession( regSessionRoutinesMetahandler, NULL );

    CMSetMetaHandler(somThis->session,kRMContainerType,regContainerMetahandler);

    container = somSelf-> OpenContainer( ev, CM_READONLY);

    //## Register partHandlerName so we can search for objects with this property

    CMProperty partHandlerNameProp =
       CMRegisterProperty( container, kRMpartHandlerNameProp);

    while ( currObject = CMGetNextObjectWithProperty( container, currObject, partHandlerNameProp ) )
    {
        partInfo = new ODPartHandlerInfoInt;
        partInfo -> InitODPartHandlerInfoIntWithStorage( ev, currObject );

        //## Just borrow the partKindInfo sequence to initialize the ODPartKind objects.
        _IDL_SEQUENCE_PartKindInfo partKinds = partInfo-> _get_partKindInfo( ev);

        for ( int i = 0; i < partKinds. _length; i++)
        {
            //## This part cheats a little bit, because a new copy of the partKindName is
            //## is not made just for the test.  If this partKindName is not found then a
            //## new ODPartKind is created which actually has a copy of the partKindName.

            ICursor* cursor = somThis-> kinds-> newCursor();
            CWrap<ODPartKind> kindWrap( testKind);
            testKind-> _set_partKindName( ev, partKinds. _buffer[i]. partKindName);

            if ( somThis-> kinds-> locate( kindWrap, *cursor) )
            {
                foundKind = somThis-> kinds-> elementAt( *cursor);
                foundKind-> Add( ev, partInfo);
            }
            else
            {
                newKind = new ODPartKind;
                newKind-> _set_partKindName( ev, newString( partKinds. _buffer[i]. partKindName));
                somThis-> kinds-> add( newKind);
                newKind-> Add( ev, partInfo);
            }
            delete cursor;
        }

        // Build a list of FileTypes

        for ( i = 0; i < partKinds. _length; i++)
        {
            //## This part cheats a little bit, because a new copy of the partFileType is
            //## is not made just for the test.  If this partFileType is not found then a
            //## new ODPartFileType is created which actually has a copy of the partFileType.

            _IDL_SEQUENCE_string types = ParseFileTypeString(partKinds. _buffer[i]);
            for ( int j = 0; j < types. _length; j++)
            {
              ICursor* cursor = somThis-> types-> newCursor();
              CWrap<ODPartFileType> typeWrap( testType);
              testType-> _set_partFileTypeName( ev, types. _buffer[j]);

              if ( somThis-> types-> locate( typeWrap, *cursor) )
              {
                  foundType = somThis-> types-> elementAt( *cursor);
                  foundType-> Add( ev, partInfo);
              }
              else
              {
                  newType = new ODPartFileType;
                  newType-> _set_partFileTypeName( ev, newString( types. _buffer[j]));
                  somThis-> types-> add( newType);
                  newType-> Add( ev, partInfo);
              }
              delete cursor;
            }
            if(types. _length)
              deleteStringSequence(types);
        }

        // Build a list of FileExtensions

        for ( i = 0; i < partKinds. _length; i++)
        {
            //## This part cheats a little bit, because a new copy of the partFileExts is
            //## is not made just for the test.  If this partFileExts is not found then a
            //## new ODPartFileExts is created which actually has a copy of the partTypeName.

            _IDL_SEQUENCE_string exts = ParseFileExtString(partKinds. _buffer[i]);
            for ( int j = 0; j < exts. _length; j++)
            {
              ICursor* cursor = somThis-> extensions-> newCursor();
              CWrap<ODPartFileExtension> extWrap( testExtension);
              testExtension-> _set_partFileExtensionName( ev, newString(exts. _buffer[j]));

              if ( somThis-> extensions-> locate( extWrap, *cursor) )
              {
                  foundExtension = somThis-> extensions-> elementAt( *cursor);
                  foundExtension-> Add( ev, partInfo);
              }
              else
              {
                  newExtension = new ODPartFileExtension;
                  newExtension-> _set_partFileExtensionName( ev, newString( exts. _buffer[j]));
                  somThis-> extensions-> add( newExtension);
                  newExtension-> Add( ev, partInfo);
              }
              delete cursor;
            }
            if(exts. _length)
              deleteStringSequence(exts);
        }

        // 123852 Build a list of categories
        for ( i = 0; i < partKinds. _length; i++)
        {
            //## This part cheats a little bit, because a new copy of the categories is
            //## is not made just for the test.  If this categories is not found then a
            //## new ODPartKind is created which actually has a copy of the category.
            //## We are also using ODPartKinds to track the category.
            _IDL_SEQUENCE_string cats = ParseCategoryString(partKinds. _buffer[i]);
            for ( int j = 0; j < cats. _length; j++)
            {
              ICursor* cursor = somThis-> categories-> newCursor();
              CWrap<ODPartKind> kindWrap( testKind);
              testKind-> _set_partKindName( ev, newString(cats. _buffer[j]));

              if ( somThis-> categories-> locate( kindWrap, *cursor) )
              {
                  foundKind = somThis-> categories -> elementAt( *cursor);
                  foundKind-> Add( ev, partInfo);
              }
              else
              {
                  newKind = new ODPartKind;
                  newKind-> _set_partKindName( ev, newString( cats. _buffer[j]));
                  somThis-> categories-> add( newKind);
                  newKind-> Add( ev, partInfo);
              }
              delete cursor;
            }
            if(cats. _length)
              deleteStringSequence(cats);
        }
        // end 123852

        ICursor* cursor = somThis-> parts-> newCursor();
        CWrap<ODPartHandlerInfoInt> partWrap( partInfo);
        if ( !somThis-> parts-> add( partWrap, *cursor ) )
        {
            removedPart = somThis-> parts-> elementAt( *cursor);
            somThis-> parts-> removeAt( *cursor);
            SOM_Assert( somThis-> parts-> add( partWrap, *cursor ), 1);

            delete removedPart;
        }
        delete cursor;
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }
  somSelf-> CloseContainer( ev);

#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    if(DosReleaseMutexSem(hmtx)) // 124840
       THROW(kODErrSemaphoreError);
#endif
#endif
//    somSelf-> ReadPreferences( ev);
}

SOM_Scope void  SOMLINK ODPartHandlerRegistryIntRemoveODPartHandlerInfoFromStorage(ODPartHandlerRegistryInt *somSelf,
                                             Environment *ev,
                                             ODPartHandlerInfoInt* partInfo,
                                             CMContainer container)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntRemoveODPartHandlerInfoFromStorage");

    //*** Local varibles ******************************************************
    CMObject                      currObject = 0;
    ISOString                     searchName;
    long                          searchNameLength;
    //*************************************************************************

    try {
    searchName = partInfo-> _get_partHandlerName( ev);

    if ( searchName)
    {
        searchNameLength = strlen( searchName) + 1;    // Null will be counted in CMSize
    }
    else
    {
        return;
    }

    //## Register partHandlerName and ISOStringType so we can search for objects with
    //## this property.

    CMProperty partHandlerNameProp =
       CMRegisterProperty( container, kRMpartHandlerNameProp);

    CMType ISOStringType           = CMRegisterType( container, kRMISOStringType);

    while ( currObject = CMGetNextObjectWithProperty( container, currObject, partHandlerNameProp ) )
    {
        CMValue value;
        CMSize  size;
        if ( value = CMUseValue( currObject, partHandlerNameProp, ISOStringType) )
        {
            //## Get value size, which includes the null terminator.  If size is > 0
            //## then allocate buffer and read data.
            if ( size = CMGetValueSize( value) )
            {
                if ( size == searchNameLength )
                {
                    string objectName = string( SOMMalloc( size));
                    SOM_Assert( (int)objectName, 1);
                    size = CMReadValueData( value, objectName, 0, size);

                    if ( !strcmp( searchName, objectName ) )
                    {
                        #if ODDebug
                        // somLPrintf( 1, "Removing: %s\n", objectName);
                        #endif
                        CMDeleteObject( currObject);
                        SOMFree( objectName);
                        break;
                    }
                    SOMFree( objectName);
                }
            }
        }
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }
}

SOM_Scope CMContainer  SOMLINK ODPartHandlerRegistryIntOpenContainer(ODPartHandlerRegistryInt *somSelf,
                                                           Environment *ev,
                                                           long flags)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntOpenContainer");


    //*** Local varibles ******************************************************
    char                          completeFileName[kMaxBufferSize];
#ifdef _PLATFORM_OS2_
    char                          objectFileName[kMaxBufferSize];
    char  *objdir;
#endif
    boolean                       exists   = 0;
    static CMRefCon               myRefCon = 0;
    CMContainerUseMode            useMode  = 0;
    ISOString                     strPtr;
#ifdef _PLATFORM_OS2_
    PCSZ                     comspecValue = NULL;
#endif
    char  *dirname;
    struct stat statbuf;
    //*************************************************************************

    strPtr = 0;
 
#ifdef _PLATFORM_OS2_
/*
    if ( !DosScanEnv( comspecVariable, &comspecValue) )
    {
        if ( comspecValue)
        {
            strcpy( completeFileName, comspecValue);
            strcat( completeFileName, ODRegistryFileName);
        }
    }  // DosScanEnv
*/

  try {
    useMode = ( flags == CM_READWRITE ) ? kCMReuseFreeSpace : 0;

    if ( ! somThis-> container)
    {
        dirname = getenv(ODRegistryPathEnvVar);
        if ((dirname == NULL) || (*dirname == 0))
        {
            getcwd((char *) &completeFileName,256);
            strcat(completeFileName, ODRegistryFileName);
        } else
        {
            strcpy(completeFileName, dirname);
            strcat(completeFileName, ODRegistryFileName);
        }
        myRefCon = createRefConForMyHandlers( somThis->session, completeFileName, NULL);
        if ( exists )
        {
            #if ODDebug
            //somLPrintf( 1, "file exists\n");
            #endif

            somThis->container = CMOpenContainer( somThis->session,
                                          myRefCon,
                                          kRMContainerType,
                                          useMode);
           if (((ContainerPtr)(somThis->container))->refCon == NULL)
           {
                   somThis->container = kODNULL;
                   return;
           }

        }
        else
        {
            HOBJECT hobj,retobj;
	    char setUpData[512];

            if ( 0 == access( completeFileName, 0) )
            {
                if ( !(retobj=WinQueryObject((PSZ) "<OD_OPENDOC>"))) {
                    retobj = WinCreateObject((PSZ) "WPFolder", (PSZ) "IBM OpenDoc",
                                                        (PSZ) "OBJECTID=<OD_OPENDOC>;",
                                                        (PSZ) "<WP_DESKTOP>", CO_FAILIFEXISTS);
                }
                objdir = getenv(ODRegistryPathEnvVar);

                for (int loop = 0;loop < 4 ;loop++ ) {
                    if ((objdir == NULL) || (*objdir == 0))
                    {
                        getcwd((char *) &objectFileName,256);
                        strcat(objectFileName, ODFileNames[loop]);
                    } else
                    {
                        strcpy(objectFileName, objdir);
                        strcat(objectFileName, ODFileNames[loop]);
                    }
                    if (! (hobj = WinQueryObject(objectFileName)))
                    {
                       somThis->container = kODNULL;
                       return;
                       
                    }
		    if ( !(WinQueryObject((PSZ) ODFolderId[loop]))) {
		      hobj = WinCreateShadow(hobj, retobj, NULL);
		      sprintf(setUpData, "OBJECTID=%s", ODFolderId[loop]);
		      WinSetObjectData(hobj, setUpData);
		    }
                } /* endfor */

            }
            FILE* f = 0;
            if ( f = fopen( completeFileName, "r") )
            {
                exists = 1;
                fclose( f);
            }

            if ( exists )
            {
                #if ODDebug
                //somLPrintf( 1, "file exists\n");
                #endif

                somThis->container = CMOpenContainer(somThis->session,
                                             myRefCon,
                                             kRMContainerType,
                                             useMode);
            }
            else
            {
                #if ODDebug
                //somLPrintf( 1, "file does not exist.\n");
                #endif

                somThis->container = CMOpenNewContainer(somThis->session,
                                                myRefCon,
                                                kRMContainerType,
                                                useMode,
                                                1, kCMDefaultEndian);
                if (((ContainerPtr)(somThis->container))->refCon == NULL)
                {
                        somThis->container = kODNULL;
                        return;
                }
            }
        }

        if ( somThis-> container == NULL )
        {
            #if ODDebug
            //somLPrintf( 0, "container not opened successfully\n");
            #endif
        }
        strcpy(somThis->completeFileName,completeFileName);
#ifdef _PLATFORM_UNIX_
        if(stat(somThis->completeFileName, &statbuf) == 0)
#else
        if(_stat(somThis->completeFileName, &statbuf) == 0)
#endif
              _flastRegUpdate = statbuf.st_atime;
        return somThis->container;
    }
  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }
#endif // OS2 platform

// start AIX code - hopefully platform non-specific

  try {
    useMode = ( flags == CM_READWRITE ) ? kCMReuseFreeSpace : 0;

    if ( ! somThis-> container)
    {
        dirname = getenv(ODRegistryPathEnvVar);
        if ((dirname == NULL) || (*dirname == 0))
        {
            getcwd((char *) &completeFileName,256);
            strcat(completeFileName, ODRegistryFileName);
        } else
        {
            strcpy(completeFileName, dirname);
            strcat(completeFileName, ODRegistryFileName);
        }
        myRefCon = createRefConForMyHandlers( somThis->session, completeFileName, NULL);
        if ( exists )
        {
            #ifdef ODDebug
            //PRINT( "file exists\n");
            #endif

            somThis->container = CMOpenContainer( somThis->session,
                                          myRefCon,
                                          kRMContainerType,
                                          useMode);
        }
        else
        {
            FILE* f = 0;
            if ( f = fopen( completeFileName, "r") )
            {
                exists = 1;
                fclose( f);
            }

            if ( exists )
            {
                #ifdef ODDebug
                //PRINT( "file exists\n");
                #endif

                somThis->container = CMOpenContainer(somThis->session,
                                             myRefCon,
                                             kRMContainerType,
                                             useMode);
            }
            else
            {
                #ifdef ODDebug
                //PRINT( "file does not exist.\n");
                #endif

                somThis->container = CMOpenNewContainer(somThis->session,
                                                myRefCon,
                                                kRMContainerType,
                                                useMode,
                                                1, kCMDefaultEndian);
                if (((ContainerPtr)(somThis->container))->refCon == NULL)
                {
                        somThis->container = kODNULL;
                        return somThis->container;
                }
            }
        }

        if ( somThis->container == NULL )
        {
#ifdef Debug
            //PRINT("container not opened successfully\n");
#endif
        }
    }
  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }
//  end AIX version

  strcpy(somThis->completeFileName,completeFileName);
#ifdef _PLATFORM_UNIX_
  if(stat(somThis->completeFileName, &statbuf) == 0)
#else
  if(_stat(somThis->completeFileName, &statbuf) == 0)
#endif
        _flastRegUpdate = statbuf.st_atime;
  return somThis->container;
}

SOM_Scope void  SOMLINK ODPartHandlerRegistryIntCloseContainer(ODPartHandlerRegistryInt *somSelf,
                                                         Environment *ev)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntCloseContainer");

    CMCloseContainer( somThis-> container);
    somThis-> container = 0;
}

SOM_Scope void  SOMLINK ODPartHandlerRegistryIntReadPreferences(ODPartHandlerRegistryInt *somSelf,
                                                            Environment *ev)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntReadPreferences");

    //*** Const varibles ******************************************************
    const string comspecVariable     = "COMSPEC";
    const string preferencesFileName = "ODPref.ini";

    //*** Local varibles ******************************************************
    ifstream                      preferencesFile( preferencesFileName);
    char                          buffer[kMaxBufferSize];
    string                        partKindName;
    string                        partHandlerName;
    char                          eatNewLine;
    char                          completeFileName[kMaxBufferSize]="ODPref.ini";
    ISOString                     strPtr;
    ISOString                     comspecValue = 0;
    //*************************************************************************

    strPtr = 0;

#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    if ( !DosScanEnv( comspecVariable, &comspecValue) )
    {
        if ( comspecValue)
        {
            strcpy( completeFileName, comspecValue);
            if ( strPtr = strrchr( completeFileName, '\\') )
            {
                ++strPtr;
            }
        }
    }

    if ( strPtr)
    {
        strcpy( strPtr, preferencesFileName);
    }
    else
    {
        strcpy( completeFileName, preferencesFileName);
    }
#endif
#endif

  try {
    preferencesFile. open( completeFileName);

    preferencesFile. getline( buffer, kMaxBufferSize);
    // throw away the first line
    int flag = 0;
    while( preferencesFile. getline( buffer, kMaxBufferSize) )
    {
        if(!strcmp(buffer, "[FILETYPES]"))
        {
           flag = 1;
           continue;
        }
        if(!strcmp(buffer, "[FILEFILTERS]"))
        {
           flag = 2;
           continue;
        }
        // 123852 added categories
        if(!strcmp(buffer, "[CATEGORIES]"))
        {
           flag = 3;
           continue;
        }
        partKindName = strtok( buffer, "=\n");
        partHandlerName = strtok( NULL, "=\n");
        switch(flag)
        {
          case 0:
            somSelf-> SetPreferredPartHandlerInList( ev, partKindName, partHandlerName);
            break;
          case 1:
            somSelf-> SetPreferredPartHandlerInTypeList( ev, partKindName, partHandlerName);
            break;
          case 2:
            somSelf-> SetPreferredPartHandlerInExtList( ev, partKindName, partHandlerName);
            break;
          case 3:
            somSelf-> SetPreferredPartHandlerInCategoryList( ev, partKindName, partHandlerName);
            break;
        }
    }
  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }
}

SOM_Scope void  SOMLINK ODPartHandlerRegistryIntWritePreferences(ODPartHandlerRegistryInt *somSelf,
                                                    Environment *ev)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntWritePreferences");

    //*** Const varibles ******************************************************
    const string comspecVariable     = "COMSPEC";
    const string preferencesFileName = "ODPref.ini";
    //*** Local varibles ******************************************************
    ofstream                      preferencesFile;
    char                          buffer[kMaxBufferSize];
    string                        partKindName;
    string                        partHandlerName;
    ICursor*                      cursor;
    char                          completeFileName[kMaxBufferSize]="ODPref.ini";
    ISOString                     strPtr;
    ISOString                     comspecValue = 0;
    //*************************************************************************

    strPtr = 0;
#if 0 // defect 22796
#ifdef _PLATFORM_OS2_

    if ( !DosScanEnv( comspecVariable, &comspecValue) )
    {
        if ( comspecValue)
        {
            strcpy( completeFileName, comspecValue);
            if ( strPtr = strrchr( completeFileName, '\\') )
            {
                ++strPtr;
            }
        }
    }

    if ( strPtr)
    {
        strcpy( strPtr, preferencesFileName);
    }
    else
    {
        strcpy( completeFileName, preferencesFileName);
    }
#endif
#endif

  try {
    preferencesFile. open( completeFileName);
    preferencesFile << "[KINDS]" << endl;

    cursor = somThis-> kinds-> newCursor();

    forCursor( *cursor)
    {
        ODPartKind* kind;
        SOM_Assert( int(kind = somThis-> kinds-> elementAt( *cursor)), 1);
        if(kind-> GetPreferredPart(ev))  // 123852
        {
        preferencesFile << kind-> _get_partKindName( ev) << "="
                        << kind-> GetPreferredPart( ev)-> _get_partHandlerName( ev)
                        << endl;
        }
    }
    delete cursor;

    preferencesFile << "[FILETYPES]" << endl;
    cursor = somThis-> types-> newCursor();

    forCursor( *cursor)
    {
        ODPartFileType* type;
        SOM_Assert( int(type = somThis-> types-> elementAt( *cursor)), 1);

        preferencesFile << type-> _get_partFileTypeName( ev) << "="
                        << type-> GetPreferredPart( ev)-> _get_partHandlerName( ev)
                        << endl;
    }
    delete cursor;

    preferencesFile << "[FILEFILTERS]" << endl;
    cursor = somThis-> extensions-> newCursor();

    forCursor( *cursor)
    {
        ODPartFileExtension* ext;
        SOM_Assert( int(ext = somThis-> extensions-> elementAt( *cursor)), 1);

        preferencesFile << ext-> _get_partFileExtensionName( ev) << "="
                        << ext-> GetPreferredPart( ev)-> _get_partHandlerName( ev)
                        << endl;
    }
    delete cursor;

    // 123852 added category preferences
    preferencesFile << "[CATEGORIES]" << endl;

    cursor = somThis-> categories-> newCursor();

    forCursor( *cursor)
    {
        ODPartKind* category;
        SOM_Assert( int(category = somThis-> categories-> elementAt( *cursor)), 1);
        if(category-> GetPreferredPart(ev))
        {
        preferencesFile << category-> _get_partKindName( ev) << "="
                        << category-> GetPreferredPart( ev)-> _get_partHandlerName( ev)
                        << endl;
        }
    }
    delete cursor;
  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }

}

SOM_Scope void  SOMLINK ODPartHandlerRegistryIntsomInit(ODPartHandlerRegistryInt *somSelf)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntsomInit");

    somThis-> container  = 0;
    somThis-> parts      = new PartSet;
    somThis-> kinds      = new KindSet;
    somThis-> types      = new FileTypeSet;
    somThis-> extensions = new FileExtensionSet;
    somThis-> categories = new KindSet;
    somThis-> completeFileName = (char *) SOMMalloc(kMaxBufferSize * sizeof(char));
#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    somThis->fHMTXBentoAccess = 0; // 124840
#endif
#endif

    //*** Constants      ******************************************************
    const kMaxBufferSize             = 1024;

    //*** Local varibles ******************************************************
    char                          completeFileName[kMaxBufferSize];
    ISOString                     strPtr;
    ISOString                     comspecValue = 0;
    //*************************************************************************

    strPtr = 0;
#if 0 // defect 22796
#ifdef _PLATFORM_OS2

    if ( !DosScanEnv( comspecVariable, &comspecValue) )
    {
        if ( comspecValue)
        {
            strcpy( completeFileName, comspecValue);
            if ( strPtr = strrchr( completeFileName, '\\') )
            {
                ++strPtr;
            }
        }
    }

    if ( strPtr)
    {
        strcpy( strPtr, ODRegistryFileName);
    }
    else
    {
        strcpy( completeFileName, ODRegistryFileName);
    }

    // [133052] - Changed from using xhandlrs to the cached "bento" handlers
    //          - (Also did away with some unneeded WritePreferences)
//#endif  // platform os2

    somThis->fPlatformFile = new PlatformFile;
    somThis->fPlatformFile->Specify((ODFileSpec*) completeFileName);

    somThis->session = CMStartSession( sessionRoutinesMetahandler, NULL );
    somThis->fHandlers = new ODFSBentoHandlers(somThis->session,
                                                  somThis->fPlatformFile );
    somThis->fHandlers->Initialize();
    somThis->_flastRegUpdate = 0;

    #if WANT_REGISTRY
    Initialize();
    #endif

//#ifdef _PLATFORM_OS2_
    /* 124840 Create Mutex semaphore to allow access to bto file */
    int rc = DosOpenMutexSem(BENTOSEMNAME, &somThis->fHMTXBentoAccess);

    if(rc == ERROR_SEM_NOT_FOUND)
    {
       if(DosCreateMutexSem(BENTOSEMNAME, &somThis->fHMTXBentoAccess, 0, FALSE))
          THROW(kODErrSemaphoreError);
    }
    else if(rc != NO_ERROR)
       THROW(kODErrSemaphoreError);
//#endif
#endif  // platform os2
#endif // defect 22796

}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntDeregisterPartHandler(ODPartHandlerRegistryInt *somSelf,
                                                     Environment *ev,
                                                     ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntDeregisterPartHandler");

    //*** Local varibles ******************************************************

    CMContainer               container;
    static ODPartHandlerInfoInt* partToRemove  = new ODPartHandlerInfoInt;
    static ODPartKind*           testKind  = new ODPartKind;
    ODPartHandlerInfoInt*        removedPart;
    ODPartKind*                  newKind;
    ODPartKind*                  foundKind;
    ICursor*                     partCursor;
    ICursor*                     kindCursor;
    CWrap<ODPartHandlerInfoInt>  partWrap( partToRemove);
#ifdef _PLATFORM_OS2_
    HMTX                         hmtx = 0;
#endif
    //*************************************************************************

#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    //## 124840 Get the Bento mutex sem to have access to partreg.bto
    if( DosOpenMutexSem(BENTOSEMNAME, &hmtx))
       THROW(kODErrSemaphoreError);

    if(DosRequestMutexSem(hmtx, SEM_INDEFINITE_WAIT))
       THROW(kODErrSemaphoreError);
#endif
#endif

  try {
    container = somSelf-> OpenContainer( ev, CM_READWRITE);
    CMObject partObject = CMNewObject( container);

    partCursor = somThis-> parts-> newCursor();

    partToRemove-> _set_partHandlerName( ev, partHandlerName);

    if ( somThis-> parts-> locate( partWrap, *partCursor) )
    {

        removedPart = somThis-> parts-> elementAt( *partCursor);
        somThis-> parts-> removeAt( *partCursor);

        //## Just borrow the partKindInfo sequence to find the ODPartKind objects.
        _IDL_SEQUENCE_PartKindInfo partKinds = removedPart-> _get_partKindInfo( ev);

        for ( int i = 0; i < partKinds. _length; i++)
        {
            //## This part cheats a little bit, because a new copy of the partKindName is
            //## is not made just for the test.

            ICursor* kindCursor = somThis-> kinds-> newCursor();
            CWrap<ODPartKind> kindWrap( testKind);

      //      testKind-> _set_partKindName( ev, newString(partKinds. _buffer[i]. partKindName));
            testKind-> _set_partKindName( ev, partKinds. _buffer[i]. partKindName);

            if ( somThis-> kinds-> locate( kindWrap, *kindCursor) )
            {
                foundKind = somThis-> kinds-> elementAt( *kindCursor);
                foundKind-> Remove( ev, removedPart);
                if ( !foundKind-> Count( ev) )
                {
                    delete foundKind;
                }
                somThis-> kinds-> removeAt( *kindCursor); // [123475]
            }
            else
            {
                //## This would be an internal error.
                SOM_Assert( 1, 1);
            }
            delete kindCursor;
        }

#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
        // remove the template for the partkind
        ODDeleteTemplate(partKinds._buffer[0]. objectID); //[143601] - pfe
#endif
#endif
#ifdef _PLATFORM_WIN32_
        // Delete from Windows side
        WindowsRegistry::UnregisterPart(removedPart-> _get_partHandlerClassName(ev));
#endif

        //****************
        // Replaced the delete to remove the part from storage <jso>
        somSelf-> RemoveODPartHandlerInfoFromStorage( ev, removedPart, container);
       // delete removedPart;
    }
    else
    {
        CMDeleteObject( partObject);
        return errPR_PART_HANDLER_NOT_FOUND;
    }

    delete partCursor; // jso
    somSelf-> CloseContainer( ev);
#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    if(DosReleaseMutexSem(hmtx)) // 124840
       THROW(kODErrSemaphoreError);
#endif
#endif

  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }
  return PR_NO_ERROR;
}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntDeregisterPartHandlerClass(ODPartHandlerRegistryInt *somSelf,
                                                   Environment *ev,
                                                   string partHandlerClassName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntDeregisterPartHandlerClass");

    //*** Local varibles ******************************************************
    ODPartHandlerInfoInt*     partInfo;
    ICursor*                  cursor;
    //*************************************************************************

    cursor = somThis-> parts-> newCursor();

    forCursor( *cursor)
    {
        SOM_Assert( int(partInfo = somThis-> parts-> elementAt( *cursor)), 1);

        if ( !strcmp( partHandlerClassName, partInfo-> _get_partHandlerClassName( ev)) )
        {
            return somSelf-> DeregisterPartHandler( ev, partInfo-> _get_partHandlerName( ev));
        }
    }
    delete cursor;

    return errPR_PART_HANDLER_NOT_FOUND;
}

SOM_Scope _IDL_SEQUENCE_ISOString  SOMLINK ODPartHandlerRegistryIntGetPartHandlerList(ODPartHandlerRegistryInt *somSelf,
                                                      Environment *ev,
                                                      ISOString partKindName,
                                                      string category)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetPartHandlerList");

    //*** Local varibles ******************************************************
    ODPartHandlerInfoInt*     partInfo;
    ICursor*                  cursor;
    _IDL_SEQUENCE_ISOString   partInfoList;
    //*************************************************************************

    somSelf->CheckAndRefresh(ev);
    partInfoList. _length = partInfoList. _maximum = somThis-> parts-> numberOfElements();

 try {
    if ( partInfoList. _length)
    {
        partInfoList. _buffer  = (string*)SOMMalloc( partInfoList. _length * sizeof( string*) );

        cursor = somThis-> parts-> newCursor();
        long i = 0;
        forCursor( *cursor)
        {
            SOM_Assert( int(partInfo = somThis-> parts-> elementAt( *cursor)), 1);

            if ( partInfo-> SupportsKind( ev, partKindName)
            &&   partInfo-> SupportsCategory( ev, category) )
            {
                partInfoList. _buffer[i++] = newString( partInfo-> _get_partHandlerName( ev));
            }
        }
        delete cursor;
        partInfoList. _length = i;
    }
    else
    {
        partInfoList. _buffer = 0;
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }

    return partInfoList;
}

SOM_Scope ISOString  SOMLINK ODPartHandlerRegistryIntGetPreferredPartHandler(ODPartHandlerRegistryInt *somSelf,
                                                     Environment *ev,
                                                     ISOString partKindName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetPreferredPartHandler");

    //*** Local varibles ******************************************************
    static ODPartKind*        testKind  = new ODPartKind;
    CWrap<ODPartKind>         kindWrap( testKind);
    ODPartKind*               foundKind;
    ICursor*                  cursor;
    //*************************************************************************

    somSelf->CheckAndRefresh(ev);
 try {
    if ( partKindName )
    {
        testKind-> _set_partKindName( ev, partKindName);

        cursor = somThis-> kinds-> newCursor();

        if ( somThis-> kinds-> locate( kindWrap, *cursor) )
        {
            foundKind = somThis-> kinds-> elementAt( *cursor);
            if(foundKind ->GetPreferredPart(ev)) // 123852
              return newString( foundKind-> GetPreferredPart( ev)-> _get_partHandlerName( ev));
            else
              return kODNULL;
        }

        delete cursor;
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }


    return 0;
}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandler(ODPartHandlerRegistryInt *somSelf,
                                                   Environment *ev,
                                                   ISOString partKindName,
                                                   ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntSetPreferredPartHandler");

    long rc;

    rc = somSelf-> SetPreferredPartHandlerInList( ev, partKindName, partHandlerName);

//    if ( rc == RM_NO_ERROR )
//    {
//        somSelf-> WritePreferences( ev);
//    }
    return rc;
}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerInList(ODPartHandlerRegistryInt *somSelf,
                                                   Environment *ev,
                                                   ISOString partKindName,
                                                   ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntSetPreferredPartHandlerInList");

    //*** Local varibles ******************************************************
    static ODPartKind*               testKind  = new ODPartKind;
    CWrap<ODPartKind>                kindWrap( testKind);
    ODPartKind*                      foundKind = 0;

    static ODPartHandlerInfoInt*     testPart  = new ODPartHandlerInfoInt;
    CWrap<ODPartHandlerInfoInt>      partWrap( testPart);
    ODPartHandlerInfoInt*            foundPart = 0;

    ICursor*                         cursor;
    long                             rc = RM_NO_ERROR;
    //*************************************************************************

  try {
    if ( partKindName )
    {
        testKind-> _set_partKindName( ev, partKindName);

        cursor = somThis-> kinds-> newCursor();

        if ( somThis-> kinds-> locate( kindWrap, *cursor) )
        {
            foundKind = somThis-> kinds-> elementAt( *cursor);
        }
        else
        {
            rc = errPR_PART_KIND_NOT_FOUND;
        }

        delete cursor;
    }
    else
    {
        rc = errPR_PART_KIND_NOT_FOUND;
    }

    if ( foundKind )
    {
        if ( partHandlerName )
        {
            testPart-> _set_partHandlerName( ev, partHandlerName);

            cursor = somThis-> parts-> newCursor();

            if ( somThis-> parts-> locate( partWrap, *cursor) )
            {
                foundPart = somThis-> parts-> elementAt( *cursor);

                foundKind-> SetPreferredPart( ev, foundPart);
            }
            else
            {
                rc = errPR_PART_HANDLER_NOT_FOUND;
            }
            delete cursor;
        }
        else
        {
            rc = errPR_PART_HANDLER_NOT_FOUND;
        }
   }
   } // try
   catch (ODException _exception) {
     SetErrorCode(kODNoError);
   }

    return rc;
}

SOM_Scope void  SOMLINK ODPartHandlerRegistryIntsomDumpSelfInt(ODPartHandlerRegistryInt *somSelf,
                                                               long level)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntsomDumpSelfInt");

    ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_somDumpSelfInt(somSelf,
                                                                         level);

    //*** Local varibles ******************************************************
    ODPartHandlerInfoInt*     partInfo;
    ICursor*                  cursor;
    //*************************************************************************

    cursor = somThis-> parts-> newCursor();

    forCursor( *cursor)
    {
        SOM_Assert( int(partInfo = somThis-> parts-> elementAt( *cursor)), 1);
        partInfo-> somDumpSelf( level+1);
    }
    delete cursor;
}

SOM_Scope void  SOMLINK ODPartHandlerRegistryIntUpdateODPartHandlerInfo(ODPartHandlerRegistryInt *somSelf,
                                       Environment *ev,
                                       ISOString partHandlerName,
                                       _IDL_SEQUENCE_PartKindInfo * newKindInfo)
{

    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntUpdateODPartHandlerInfo");

    //*** Local varibles ******************************************************
    CMContainer               container;
    static ODPartHandlerInfoInt*        testPart  = new ODPartHandlerInfoInt;
    ODPartHandlerInfoInt*               foundPart;
    ICursor*                            cursor;
    CWrap<ODPartHandlerInfoInt>         partWrap( testPart);
    CMObject                      currObject = 0;
    long                          searchNameLength;
#ifdef _PLATFORM_OS2_
    HMTX                          hmtx = 0;
#endif
    //*************************************************************************



#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    //## 124840 Get the Bento mutex sem to have access to partreg.bto
    if( DosOpenMutexSem(BENTOSEMNAME, &hmtx))
       THROW(kODErrSemaphoreError);

    if(DosRequestMutexSem(hmtx, SEM_INDEFINITE_WAIT))
       THROW(kODErrSemaphoreError);
#endif
#endif

  try {
    container = somSelf-> OpenContainer( ev, CM_READWRITE);
    CMObject partObject = CMNewObject( container);


    if ( partHandlerName)
    {
        searchNameLength = strlen( partHandlerName) + 1;    // Null will be counted in CMSize
    }
    else
    {
        return;
    }

    //## Register partHandlerName and ISOStringType so we can search for objects with
    //## this property.

    CMProperty partHandlerNameProp =
       CMRegisterProperty( container, kRMpartHandlerNameProp);

    CMType ISOStringType           = CMRegisterType( container, kRMISOStringType);

    while ( currObject = CMGetNextObjectWithProperty( container, currObject, partHandlerNameProp ) )
    {
        CMValue value;
        CMSize  size;
        if ( value = CMUseValue( currObject, partHandlerNameProp, ISOStringType) )
        {
            //## Get value size, which includes the null terminator.  If size is > 0
            //## then allocate buffer and read data.
            if ( size = CMGetValueSize( value) )
            {
                if ( size == searchNameLength )
                {
                    string objectName = string( SOMMalloc( size));
                    SOM_Assert( (int)objectName, 1);
                    size = CMReadValueData( value, objectName, 0, size);

                    if ( !strcmp( partHandlerName, objectName ) )
                    {
                        #if ODDebug
                        //somLPrintf( 1, "Updating: %s\n", objectName);
                        #endif
                        cursor = somThis-> parts-> newCursor();

                        testPart-> _set_partHandlerName( ev, partHandlerName);

                        if ( somThis-> parts-> locate( partWrap, *cursor) )
                        {
                            foundPart = somThis-> parts-> elementAt( *cursor);
                            foundPart-> WriteODPartHandlerKindInfo( ev, currObject, newKindInfo);
                        }
                        delete cursor;
                        SOMFree( objectName);
                        break;
                    }
                    SOMFree( objectName);
                }
            }
        }
    }

    somSelf-> CloseContainer( ev);
  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }

#if 0 // defect 22796
#ifdef _PLATFORM_OS2_
    if(DosReleaseMutexSem(hmtx)) // 124840
       THROW(kODErrSemaphoreError);
#endif
#endif
    // end



}

SOM_Scope ISOString  SOMLINK ODPartHandlerRegistryIntGetPreferredPartHandlerForFileType(ODPartHandlerRegistryInt *somSelf,
                                                   Environment *ev,
                                                   ISOString partFileTypeName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetPreferredPartHandlerForFileType");

    //*** Local varibles ******************************************************
    static ODPartFileType*    testType  = new ODPartFileType;
    CWrap<ODPartFileType>     typeWrap( testType);
    ODPartFileType*           foundType;
    ICursor*                  cursor;
    //*************************************************************************

    somSelf->CheckAndRefresh(ev);
  try {
    if ( partFileTypeName )
    {
        testType-> _set_partFileTypeName( ev, partFileTypeName);

        cursor = somThis-> types-> newCursor();

        if ( somThis-> types-> locate( typeWrap, *cursor) )
        {
            foundType = somThis-> types-> elementAt( *cursor);
            return newString( foundType-> GetPreferredPart( ev)-> _get_partHandlerName( ev));
        }

        delete cursor;
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }

    return 0;
}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerForFileType(ODPartHandlerRegistryInt *somSelf,
                                                   Environment *ev,
                                                   ISOString partFileTypeName,
                                                   ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntSetPreferredPartHandlerForFileType");

    long rc;

    rc = somSelf-> SetPreferredPartHandlerInTypeList( ev, partFileTypeName, partHandlerName);

//    if ( rc == RM_NO_ERROR )
//    {
//        somSelf-> WritePreferences( ev);
//    }
    return rc;
}

SOM_Scope ISOString  SOMLINK ODPartHandlerRegistryIntGetPreferredPartHandlerForFileExt(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString partFileExtensionName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetPreferredPartHandlerForFileExt");

    //*** Local varibles ******************************************************
    static ODPartFileExtension*    testExtension  = new ODPartFileExtension;
    CWrap<ODPartFileExtension>     extensionWrap( testExtension);
    ODPartFileExtension*           foundExtension;
    ICursor*                  cursor;
    //*************************************************************************

    somSelf->CheckAndRefresh(ev);
  try {
    if ( partFileExtensionName )
    {
        testExtension-> _set_partFileExtensionName( ev, partFileExtensionName);

        cursor = somThis-> extensions-> newCursor();

        if ( somThis-> extensions-> locate( extensionWrap, *cursor) )
        {
            foundExtension = somThis-> extensions-> elementAt( *cursor);
            return newString( foundExtension-> GetPreferredPart( ev)-> _get_partHandlerName( ev));
        }

	// defect 27025: do not delete cset cursors
        //delete cursor;
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }


    return 0;
}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerForFileExt(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString partFileExtensionName,
                                              ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntSetPreferredPartHandlerForFileExt");

    long rc;

    rc = somSelf-> SetPreferredPartHandlerInExtList( ev, partFileExtensionName, partHandlerName);

//    if ( rc == RM_NO_ERROR )
//    {
//        somSelf-> WritePreferences( ev);
//    }
    return rc;
}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerInTypeList(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString partFileTypeName,
                                              ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntSetPreferredPartHandlerInTypeList");

    //*** Local varibles ******************************************************
    static ODPartFileType*           testType  = new ODPartFileType;
    CWrap<ODPartFileType>            typeWrap( testType);
    ODPartFileType*                  foundType = 0;

    static ODPartHandlerInfoInt*     testPart  = new ODPartHandlerInfoInt;
    CWrap<ODPartHandlerInfoInt>      partWrap( testPart);
    ODPartHandlerInfoInt*            foundPart = 0;

    ICursor*                         cursor;
    long                             rc = RM_NO_ERROR;
    //*************************************************************************

  try {
    if ( partFileTypeName )
    {
        testType-> _set_partFileTypeName( ev, partFileTypeName);

        cursor = somThis-> types-> newCursor();

        if ( somThis-> types-> locate( typeWrap, *cursor) )
        {
            foundType = somThis-> types-> elementAt( *cursor);
        }
        else
        {
            rc = errPR_PART_KIND_NOT_FOUND;
        }

        delete cursor;
    }
    else
    {
        rc = errPR_PART_KIND_NOT_FOUND;
    }

    if ( foundType )
    {
        if ( partHandlerName )
        {
            testPart-> _set_partHandlerName( ev, partHandlerName);

            cursor = somThis-> parts-> newCursor();

            if ( somThis-> parts-> locate( partWrap, *cursor) )
            {
                foundPart = somThis-> parts-> elementAt( *cursor);

                foundType-> SetPreferredPart( ev, foundPart);
            }
            else
            {
                rc = errPR_PART_HANDLER_NOT_FOUND;
            }
            delete cursor;
        }
        else
        {
            rc = errPR_PART_HANDLER_NOT_FOUND;
        }
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }

    return rc;
}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerInExtList(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString partFileExtensionName,
                                              ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntSetPreferredPartHandlerInExtList");

    //*** Local varibles ******************************************************
    static ODPartFileExtension*           testExtension  = new ODPartFileExtension;
    CWrap<ODPartFileExtension>            extensionWrap( testExtension);
    ODPartFileExtension*                  foundExtension = 0;

    static ODPartHandlerInfoInt*     testPart  = new ODPartHandlerInfoInt;
    CWrap<ODPartHandlerInfoInt>      partWrap( testPart);
    ODPartHandlerInfoInt*            foundPart = 0;

    ICursor*                         cursor;
    long                             rc = RM_NO_ERROR;
    //*************************************************************************

  try {
    if ( partFileExtensionName )
    {
        testExtension-> _set_partFileExtensionName( ev, partFileExtensionName);

        cursor = somThis-> extensions-> newCursor();

        if ( somThis-> extensions-> locate( extensionWrap, *cursor) )
        {
            foundExtension = somThis-> extensions-> elementAt( *cursor);
        }
        else
        {
            rc = errPR_PART_KIND_NOT_FOUND;
        }

        delete cursor;
    }
    else
    {
        rc = errPR_PART_KIND_NOT_FOUND;
    }

    if ( foundExtension )
    {
        if ( partHandlerName )
        {
            testPart-> _set_partHandlerName( ev, partHandlerName);

            cursor = somThis-> parts-> newCursor();

            if ( somThis-> parts-> locate( partWrap, *cursor) )
            {
                foundPart = somThis-> parts-> elementAt( *cursor);

                foundExtension-> SetPreferredPart( ev, foundPart);
            }
            else
            {
                rc = errPR_PART_HANDLER_NOT_FOUND;
            }
            delete cursor;
        }
        else
        {
            rc = errPR_PART_HANDLER_NOT_FOUND;
        }
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }


    return rc;
}

SOM_Scope _IDL_SEQUENCE_string  SOMLINK ODPartHandlerRegistryIntGetFileTypeList(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString partHandlerName,
                                              ISOString partKindName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetFileTypeList");

    //*** Local varibles ******************************************************
    _IDL_SEQUENCE_string                partTypes;
    _IDL_SEQUENCE_PartKindInfo          partKindInfo;
    ICursor*                            cursor;
    ODPartHandlerInfoInt*               foundPart;
    static ODPartHandlerInfoInt*        testPart  = new ODPartHandlerInfoInt;
    CWrap<ODPartHandlerInfoInt>         partWrap( testPart);
    //*************************************************************************

    partTypes. _length = partTypes. _maximum = 0;
    partTypes. _buffer = 0;

  try {
    somSelf->CheckAndRefresh(ev);
    if ( partHandlerName)
    {
        testPart-> _set_partHandlerName( ev, partHandlerName);

        cursor = somThis-> parts-> newCursor();

        if ( somThis-> parts-> locate( partWrap, *cursor) )
        {
            foundPart = somThis-> parts-> elementAt( *cursor);
            partKindInfo = foundPart-> _get_partKindInfo( ev);

            if ( partKindName )
            {
                long j = 0;
                for ( long i = 0; i < partKindInfo. _length; i++)
                {
                  if(!strcmp(partKindName, partKindInfo. _buffer[j].partKindName))
                  {
                    partTypes = ParseFileTypeString(partKindInfo. _buffer[j]);
                    return partTypes;
                  }
                }
            }
            else
            {
                int count = CountFileTypes(partKindInfo);
                int bufCount = 0;
                _IDL_SEQUENCE_string types;
                types. _buffer = 0;
                types. _length = 0;
                partTypes. _buffer  = (string*)SOMMalloc( count * sizeof( string*) );
                if(types. _length)
                {
                  for ( long j = 0; j < partKindInfo. _length; j++)
                  {
                    types = ParseFileTypeString(partKindInfo. _buffer[j]);;
                    for( int k = 0; k < types. _length; k++);
                    {
                      partTypes. _buffer[bufCount++] = newString(types. _buffer[k]);
                    }
                   deleteStringSequence(types);
                  }
                }
            }
        }
        delete cursor;
    }
    else
    {

        partTypes. _maximum = partTypes. _length = somThis-> types-> numberOfElements();

        if ( partTypes. _length)
        {
            partTypes. _buffer  = (string*)SOMMalloc( partTypes. _length * sizeof( string*) );

            long i = 0;
            cursor = somThis-> types-> newCursor();

            forCursor( *cursor)
            {
                ODPartFileType* fileType;
                SOM_Assert( int(fileType = somThis-> types-> elementAt( *cursor)), 1);

                partTypes. _buffer[i++] = newString( fileType-> _get_partFileTypeName( ev));
            }

            delete cursor;
            partTypes. _length = i;

        }
    }
  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }

  return partTypes;
}

SOM_Scope _IDL_SEQUENCE_string  SOMLINK ODPartHandlerRegistryIntGetFileExtensionList(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString partHandlerName,
                                              ISOString partKindName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetFileExtensionList");

    //*** Local varibles ******************************************************
    _IDL_SEQUENCE_string             partExtensions;
    _IDL_SEQUENCE_PartKindInfo          partKindInfo;
    ICursor*                            cursor;
    ODPartHandlerInfoInt*               foundPart;
    static ODPartHandlerInfoInt*        testPart  = new ODPartHandlerInfoInt;
    CWrap<ODPartHandlerInfoInt>         partWrap( testPart);
    //*************************************************************************

    partExtensions. _length = partExtensions. _maximum = 0;
    partExtensions. _buffer = 0;

  try {
    somSelf->CheckAndRefresh(ev);
    if ( partHandlerName)
    {
        testPart-> _set_partHandlerName( ev, partHandlerName);

        cursor = somThis-> parts-> newCursor();

        if ( somThis-> parts-> locate( partWrap, *cursor) )
        {
            foundPart = somThis-> parts-> elementAt( *cursor);
            partKindInfo = foundPart-> _get_partKindInfo( ev);

            if ( partKindName )
            {
                long j = 0;
                for ( long i = 0; i < partKindInfo. _length; i++)
                {
                  if(!strcmp(partKindName, partKindInfo. _buffer[j].partKindName))
                  {
                    partExtensions = ParseFileExtString(partKindInfo. _buffer[j]);
                    return partExtensions;
                  }
                }
            }
            else
            {
                int count = CountFileExts(partKindInfo);
                int bufCount = 0;
                _IDL_SEQUENCE_string exts;
                exts. _buffer = 0;
                exts. _length = 0;
                partExtensions. _buffer  = (string*)SOMMalloc( count * sizeof( string*) );
                for ( long j = 0; j < partKindInfo. _length; j++)
                {
                  exts = ParseFileExtString(partKindInfo. _buffer[j]);
                  if(exts. _length)
                  {
                    for( int k = 0; k < exts. _length; k++);
                    {
                      partExtensions. _buffer[bufCount++] = newString(exts. _buffer[k]);
                    }
                    deleteStringSequence(exts);
                  }
                }


            }
        }
        delete cursor;
    }
    else
    {

        partExtensions. _maximum = partExtensions. _length = somThis-> extensions-> numberOfElements();

        if ( partExtensions. _length)
        {
            partExtensions. _buffer  = (string*)SOMMalloc( partExtensions. _length * sizeof( string*) );

            long i = 0;
            cursor = somThis-> extensions-> newCursor();

            forCursor( *cursor)
            {
                ODPartFileExtension* fileExt;
                SOM_Assert( int(fileExt = somThis-> extensions-> elementAt( *cursor)), 1);

                partExtensions. _buffer[i++] = newString( fileExt-> _get_partFileExtensionName( ev));
            }

            delete cursor;
            partExtensions. _length = i;

        }
    }
  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }

   return partExtensions;
}

SOM_Scope _IDL_SEQUENCE_ISOString  SOMLINK ODPartHandlerRegistryIntGetPartHandlerListForFileTypes(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              string fileType)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetPartHandlerListForFileTypes");

    //*** Local varibles ******************************************************
    ODPartHandlerInfoInt*     partInfo;
    ICursor*                  cursor;
    _IDL_SEQUENCE_ISOString   partInfoList;
    //*************************************************************************
 
  try {
    somSelf->CheckAndRefresh(ev);
    partInfoList. _length = partInfoList. _maximum = somThis-> parts-> numberOfElements();

    if ( partInfoList. _length)
    {
        partInfoList. _buffer  = (string*)SOMMalloc( partInfoList. _length * sizeof( string*) );

        cursor = somThis-> parts-> newCursor();
        long i = 0;
        forCursor( *cursor)
        {
            SOM_Assert( int(partInfo = somThis-> parts-> elementAt( *cursor)), 1);

            if ( partInfo-> SupportsFileType( ev, fileType) )
            {
                partInfoList. _buffer[i++] = newString( partInfo-> _get_partHandlerName( ev));
            }
        }
        delete cursor;
        partInfoList. _length = i;
    }
    else
    {
        partInfoList. _length = 0;
        partInfoList. _buffer = 0;
    }
  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }

    return partInfoList;
}

SOM_Scope _IDL_SEQUENCE_ISOString  SOMLINK ODPartHandlerRegistryIntGetPartHandlerListForFileExt(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              string fileExtension)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetPartHandlerListForFileExt");

    //*** Local varibles ******************************************************
    ODPartHandlerInfoInt*     partInfo;
    ICursor*                  cursor;
    _IDL_SEQUENCE_ISOString   partInfoList;
    //*************************************************************************

  try {
    somSelf->CheckAndRefresh(ev);
    partInfoList. _length = partInfoList. _maximum = somThis-> parts-> numberOfElements();

    if ( partInfoList. _length)
    {
        partInfoList. _buffer  = (string*)SOMMalloc( partInfoList. _length * sizeof( string*) );

        cursor = somThis-> parts-> newCursor();
        long i = 0;
        forCursor( *cursor)
        {
            SOM_Assert( int(partInfo = somThis-> parts-> elementAt( *cursor)), 1);

            if ( partInfo-> SupportsFileExtension( ev, fileExtension) )
            {
                partInfoList. _buffer[i++] = newString( partInfo-> _get_partHandlerName( ev));
            }
        }
        delete cursor;
        partInfoList. _length = i;
    }
    else
    {
        partInfoList. _length = 0;
        partInfoList. _buffer = 0;
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }

    return partInfoList;
}
// Added functions for defect 123852
SOM_Scope ISOString  SOMLINK ODPartHandlerRegistryIntGetPreferredPartHandlerForCategory(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString category)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetPreferredPartHandlerForCategory");

    //*** Local varibles ******************************************************
    static ODPartKind*        testCategory  = new ODPartKind;
    CWrap<ODPartKind>         kindWrap( testCategory);
    ODPartKind*               foundCategory;
    ICursor*                  cursor;
    //*************************************************************************

    // Note the category set uses the PartKind class

  try {
    somSelf->CheckAndRefresh(ev);
    if ( category )
    {
        testCategory-> _set_partKindName( ev, category);

        cursor = somThis-> categories -> newCursor();

        if ( somThis-> categories-> locate( kindWrap, *cursor) )
        {
            foundCategory = somThis-> categories-> elementAt( *cursor);
            if(foundCategory ->GetPreferredPart(ev))
              return newString( foundCategory-> GetPreferredPart( ev)-> _get_partHandlerName( ev));
            else
              return kODNULL;
        }

        delete cursor;
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }

    return 0;

}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerForCategory(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString category,
                                              ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntSetPreferredPartHandlerForCategory");

    long rc;

    rc = somSelf-> SetPreferredPartHandlerInCategoryList( ev, category, partHandlerName);

//    if ( rc == RM_NO_ERROR )
//    {
//        somSelf-> WritePreferences( ev);
//    }
    return rc;

}

SOM_Scope long  SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerInCategoryList(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString category,
                                              ISOString partHandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntSetPreferredPartHandlerInCategoryList");

    //*** Local varibles ******************************************************
    static ODPartKind*               testCategory  = new ODPartKind;
    CWrap<ODPartKind>                kindWrap( testCategory);
    ODPartKind*                      foundCategory = 0;

    static ODPartHandlerInfoInt*     testPart  = new ODPartHandlerInfoInt;
    CWrap<ODPartHandlerInfoInt>      partWrap( testPart);
    ODPartHandlerInfoInt*            foundPart = 0;

    ICursor*                         cursor;
    long                             rc = RM_NO_ERROR;
    //*************************************************************************

  try {
    if ( category )
    {
        testCategory-> _set_partKindName( ev, category);

        cursor = somThis-> categories-> newCursor();

        if ( somThis-> categories-> locate( kindWrap, *cursor) )
        {
            foundCategory = somThis-> categories-> elementAt( *cursor);
        }
        else
        {
            rc = errPR_PART_KIND_NOT_FOUND;
        }

        delete cursor;
    }
    else
    {
        rc = errPR_PART_KIND_NOT_FOUND;
    }

    if ( foundCategory )
    {
        if ( partHandlerName )
        {
            testPart-> _set_partHandlerName( ev, partHandlerName);

            cursor = somThis-> parts-> newCursor();

            if ( somThis-> parts-> locate( partWrap, *cursor) )
            {
                foundPart = somThis-> parts-> elementAt( *cursor);

                foundCategory-> SetPreferredPart( ev, foundPart);
            }
            else
            {
                rc = errPR_PART_HANDLER_NOT_FOUND;
            }
            delete cursor;
        }
        else
        {
            rc = errPR_PART_HANDLER_NOT_FOUND;
        }
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }

    return rc;

}

SOM_Scope _IDL_SEQUENCE_string  SOMLINK ODPartHandlerRegistryIntGetCategoryList(ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString partHandlerName,
                                              ISOString partKindName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetCategoryList");

    //*** Local varibles ******************************************************
    _IDL_SEQUENCE_string                partCategories;
    _IDL_SEQUENCE_PartKindInfo          partKindInfo;
    ICursor*                            cursor;
    ODPartHandlerInfoInt*               foundPart;
    static ODPartHandlerInfoInt*        testPart  = new ODPartHandlerInfoInt;
    CWrap<ODPartHandlerInfoInt>         partWrap( testPart);

    ODPartHandlerInfoInt*               partInfo;
    _IDL_SEQUENCE_ISOString             partInfoList;
    //*************************************************************************

  try {
    somSelf->CheckAndRefresh(ev);
    partCategories. _length = partCategories. _maximum = 0;
    partCategories. _buffer = 0;

    if ( partHandlerName)
    {
        testPart-> _set_partHandlerName( ev, partHandlerName);

        cursor = somThis-> parts-> newCursor();

        if ( somThis-> parts-> locate( partWrap, *cursor) )
        {
            foundPart = somThis-> parts-> elementAt( *cursor);
            partKindInfo = foundPart-> _get_partKindInfo( ev);

            if ( partKindName )
            {
                long j = 0;
                for ( long i = 0; i < partKindInfo. _length; i++)
                {
                  if(!strcmp(partKindName, partKindInfo. _buffer[j].partKindName))
                  {
                    partCategories = ParseCategoryString(partKindInfo. _buffer[j]);
                    return partCategories;
                  }
                }
            }
            else
            {
                int count = CountCategories(partKindInfo);
                int bufCount = 0;
                _IDL_SEQUENCE_string cats;
                cats. _buffer = 0;
                cats. _length = 0;
                partCategories. _buffer  = (string*)SOMMalloc( count * sizeof( string*) );
                partCategories. _length = partCategories. _maximum = count;
                for ( long j = 0; j < partKindInfo. _length; j++)
                {
                  cats = ParseCategoryString(partKindInfo. _buffer[j]);
                  if(cats. _length)
                  {
                    for( int k = 0; k < cats. _length; k++)
                    {
                      partCategories. _buffer[bufCount++] = newString(cats. _buffer[k]);
                    }
                    deleteStringSequence(cats);
                  }
                }


            }
        }
        delete cursor;
    }
    else
    {
        _IDL_SEQUENCE_string cats;

        cursor = somThis-> parts-> newCursor();
        long totalcnt = 0;
        partCategories. _buffer  = (string*)SOMMalloc( kMaxBufferSize * sizeof( string*) );
        forCursor( *cursor)
        {
            SOM_Assert( int(partInfo = somThis-> parts-> elementAt( *cursor)), 1);
            cats. _buffer = 0;
            cats. _length = 0;
            partKindInfo = partInfo-> _get_partKindInfo( ev);

            for ( long j = 0; j < partKindInfo. _length; j++) {
                if (partKindName) {
                    if ( partInfo-> SupportsKind( ev, partKindName)) {
                         cats = ParseCategoryString(partKindInfo. _buffer[j]);
                    }
                } else {
                     cats = ParseCategoryString(partKindInfo. _buffer[j]);
                }
                if(cats. _length) {
                    int k;
                    for( k = 0; k < cats. _length; k++) {
                      partCategories. _buffer[totalcnt++] = newString(cats. _buffer[k]);
                    }
                }
            }
        }
        partCategories. _length = partCategories. _maximum = totalcnt;
        if (totalcnt) {
             deleteStringSequence(cats);
        } 
        delete cursor;

    }
  } // try
  catch (ODException _exception) {
       SetErrorCode(kODNoError);
  }

  return partCategories;
}
// end changes defect 123852

SOM_Scope ISOString  SOMLINK ODPartHandlerRegistryIntGetKindDisplayNameFromKind
                                                (ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString partKindName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetKindDisplayNameFromKind");

    //*** Local varibles ******************************************************
    ODPartHandlerInfoInt*     partInfo;
    ICursor*                  cursor;
    _IDL_SEQUENCE_ISOString   partInfoList;
    _IDL_SEQUENCE_PartKindInfo partKindInfo;
    //*************************************************************************

  try {
    somSelf->CheckAndRefresh(ev);
    int cnt = somThis-> parts-> numberOfElements();

    if ( !partKindName )
    {
        return  newString("GetKindDisplayName: partKindName == NULL");
    }

    if ( cnt)
    {
        cursor = somThis-> parts-> newCursor();
        long i = 0;
        forCursor( *cursor)
        {
            SOM_Assert( int(partInfo = somThis-> parts-> elementAt( *cursor)), 1);

            if ( partInfo-> SupportsKind( ev, partKindName) )
            {
                partKindInfo = partInfo->_get_partKindInfo(ev);
                for ( long i = 0; i <  partKindInfo. _length; i++)
                {
                    if ( !strcmp( partKindInfo. _buffer[i]. partKindName, partKindName) )
                    {
#if ODDebug
        //PRINT("GetKindDisplayName: partKindName found\n");
#endif
                        return  newString(partKindInfo. _buffer[i]. partKindDisplayName);
                    }
                }
            }
        }
        delete cursor;
        return newString("GetKindDisplayName: No PartKind matches partKindName");
    }
    else
    {
        return newString("GetKindDisplayName: No Parts found");
    }
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }


}

SOM_Scope ISOString  SOMLINK ODPartHandlerRegistryIntGetHandlerDisplayNameFromHandler
                                                (ODPartHandlerRegistryInt *somSelf,
                                              Environment *ev,
                                              ISOString HandlerName)
{
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntGetHandlerDisplayNameFromHandler");

    //*** Local varibles ******************************************************
    ICursor*                            cursor;
    ODPartHandlerInfoInt*               foundPart;
    ODPartHandlerInfoInt*               testPart  = new ODPartHandlerInfoInt;
    CWrap<ODPartHandlerInfoInt>         partWrap( testPart);
    //*************************************************************************

  try {
    somSelf->CheckAndRefresh(ev);
    if ( HandlerName)
    {
        testPart-> _set_partHandlerName( ev, HandlerName);

        cursor = somThis-> parts-> newCursor();

        if ( somThis-> parts-> locate( partWrap, *cursor) )
        {
            foundPart = somThis-> parts-> elementAt( *cursor);
            delete cursor;
            return newString(foundPart->_get_partHandlerDisplayName(ev));
        }
        delete cursor;
    }
    else
    {
        return newString("GetHandlerDisplayName: partHandlerName==NULL");
    }
    return newString("GetHandlerDisplayName: partHandlerName NOT found");
  } // try
  catch (ODException _exception) {
    SetErrorCode(kODNoError);
  }


}


SOM_Scope void  SOMLINK ODPartHandlerRegistryIntCheckAndRefresh(ODPartHandlerRegistryInt *somSelf,
                                                            Environment *ev)

{ 
    ODPartHandlerRegistryIntData *somThis = ODPartHandlerRegistryIntGetData(somSelf);
    ODPartHandlerRegistryIntMethodDebug("ODPartHandlerRegistryInt","ODPartHandlerRegistryIntCheckAndRefresh");

   struct stat statbuf;
   long update = 0;
   char completeRegFileName[256];

#ifdef _PLATFORM_UNIX_
  if(stat(somThis->completeFileName, &statbuf) == 0)
#else
  if(_stat(somThis->completeFileName, &statbuf) == 0)
#endif

   {
        update = statbuf.st_atime;
        if (update > _flastRegUpdate)
        {
                _flastRegUpdate = update;
                SOMClass * ODClass = ODPartHandlerRegistryIntNewClass
                                     (ODPartHandlerRegistryInt_MajorVersion,
                                      ODPartHandlerRegistryInt_MinorVersion);
                somSelf->somUninit();
                somSelf = (ODPartHandlerRegistryInt *)
                          ODClass->somRenew(somSelf);
                somSelf->InitODPartHandlerRegistryIntWithStorage(ev);
        }
   }
}
