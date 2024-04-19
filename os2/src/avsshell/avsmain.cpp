/* @(#)Z 1.5 os2/src/avsshell/avsmain.cpp, oddataxfer, od96os2, odos29646d 96/11/15 15:50:54 (96/08/23 01:40:24) */
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

//
// Copyright (C) Apple Computer, Inc., 1994
//
//******************************************************************

// Commented out for now as OSA support is not yet defined. 
// #define INCL_OSAAPI      
// #define INCL_OSA         
#include <os2.h>

#ifndef _ODTYPESB_
#include <ODTypesB.xh>
#endif

#ifndef _ODTYPESF_
#include <ODTypesF.h>
#endif

#ifndef _ALTPOINT_
#include <AltPoint.h>
#endif

#ifndef _ODTYPESM_
#include <ODTypesM.xh>
#endif

#ifndef _ODTYPES_
   #include "ODTypes.h"
#endif

#ifndef _ODMEMORY_
   #include <ODMemory.h>
#endif

#ifndef _EXCEPT_
   #include <Except.h>
#endif

#if VALIDATE_EVERYTHING
   #include "MemDebg.h"
#endif

#include "AvsShell.h"

#ifdef _PLATFORM_OS2_
// ODSmInit not found in tree, ODInitExceptions is found in ODDebug.h
//#ifndef _ODSMINIT_
//#include <ODSmInit.h>
//#endif
#ifndef _ODDEBUG_
#include <ODDebug.h>
#endif
#endif


int main ( int argc, char *argv[])
{
   #ifndef _PLATFORM_OS2_
      ... code removed ...
   #else
     #if 0
       char *fileName = 0;
       if ( argc > 1)
       {
         fileName = argv[1];
       }
     #endif

     ODInitExceptions();    // Sets up SOMError and SOMPrintf
#ifdef _PLATFORM_OS2_
     #if ODDebug
        SetOutputMode(kWriteToFile);
     #else
        SetOutputMode(kNoOutput);
     #endif
#endif

     #if 1
        #if VALIDATE_EVERYTHING
            ODBeginMemValidation();
        #endif

          AvsShell*  le = new AvsShell;
          le->go( argc, argv );
          delete le;

        #if VALIDATE_EVERYTHING
          ODEndMemValidation();
        #endif
     #else
        ODShell *somShell;
        Environment *ev = somGetGlobalEnvironment();

        somShell = new ODShell;

        somShell->Run(ev, fileName);

        delete somShell;
     #endif

     return 0;
   #endif
}


