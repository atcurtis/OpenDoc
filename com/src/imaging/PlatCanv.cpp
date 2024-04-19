/* @(#)Z 1.9 com/src/imaging/PlatCanv.cpp, odimaging, od96os2, odos29646d 96/11/15 15:48:47 (96/10/29 09:25:15) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odimaging
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
/*         05/15/96 jab Added OS/2 specific code                    */
/*         05/10/96 jab Add GetPlatformCanvasType                   */
/*         03/20/96 jab PlatformCanvas is now a platform-independent*/
/*                      parent class for all platform canvases.     */
/*  130552 07/22/95 ced Add SetPS, HasWindow, GetWindow methods     */
/*  124941 07/11/95 ced Add Exception handling                      */
/*                                                                  */
/********************************************************************/

#ifndef SOM_Module_PlatCanv_Source
#define SOM_Module_PlatCanv_Source
#endif
#define ODPlatformCanvas_Class_Source
#define VARIABLE_MACROS


#ifndef SOM_ODPlatformCanvas_xih
#include "PlatCanv.xih"
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _ERRORDEF_
#include "ErrorDef.xh"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif 





#ifdef _PLATFORM_WIN32_

#ifndef SOM_ODWin32Canvas_xih
#include "winpcanv.xih"
#endif

#endif  // _PLATFORM_WIN32_


#ifdef _PLATFORM_OS2_

#ifndef SOM_ODOS2Canvas_xih
#include "os2pcanv.xih"
#endif

#endif  // _PLATFORM_OS2_

#ifdef _PLATFORM_AIX_

#ifndef SOM_ODAIXCanvas_xih
#include "AIXPCanv.xih"
#endif

#endif  // _PLATFORM_AIX_




SOM_Scope ODPlatformCanvas*  SOMLINK ODPlatformCanvasCreateConcreteCanvas( ODPlatformCanvas* somSelf,
                                                               Environment *ev,
                                                               ODGraphicsSystem aSystemType,
                                                               ODBoolean isDynamic,
                                                               ODBoolean isOffScreen )
{
    ODPlatformCanvasMethodDebug("ODPlatformCanvas","ODPlatformCanvasCreateConcreteCanvas");

    ODPlatformCanvas *c = kODNULL;  ODVolatile( c );

#ifdef _PLATFORM_WIN32_
    ODWin32Canvas *pc = kODNULL; ODVolatile( pc );
#endif

#ifdef _PLATFORM_OS2_
    ODOS2Canvas *pc = kODNULL; ODVolatile( pc );
#endif

#ifdef _PLATFORM_AIX_
    ODAIXCanvas *pc = kODNULL; ODVolatile( pc );
#endif


    SOM_TRY

#ifdef _PLATFORM_WIN32_
       pc = new ODWin32Canvas;
#endif  // _PLATFORM_WIN32_


#ifdef _PLATFORM_OS2_
       pc = new ODOS2Canvas;
#endif  // _PLATFORM_OS2_


#ifdef _PLATFORM_AIX_
       pc = new ODAIXCanvas; 
#endif  // _PLATFORM_AIX_

       THROW_IF_NULL( pc ); 

       c = pc->CreateConcreteCanvas( ev, aSystemType, isDynamic, isOffScreen ); 
       THROW_IF_NULL( c );
    
       ODDeleteObject( pc ); 

       return c;


    SOM_CATCH_ALL{
       if( c )
          ODDeleteObject( c );
       if( pc )  
          ODDeleteObject( pc ); 
       return kODNULL;
    }
    SOM_ENDTRY
}



SOM_Scope ODPlatformCanvasType  SOMLINK ODPlatformCanvasGetPlatformCanvasType( ODPlatformCanvas *somSelf, 
                                                          Environment *ev ) 
{
    ODPlatformCanvasMethodDebug("ODPlatformCanvas","ODPlatformCanvasGetPlatformCanvasType");

    return kODNoPlatformCanvas;
}


SOM_Scope ODGraphicsSystem  SOMLINK ODPlatformCanvasGetSystemType( ODPlatformCanvas *somSelf, 
                                                          Environment *ev ) 
{
    ODPlatformCanvasMethodDebug("ODPlatformCanvas","ODPlatformCanvasGetSystemType");

    return kODNoGraphicsSystem;
}



SOM_Scope ODBoolean  SOMLINK ODPlatformCanvasHasWindow( ODPlatformCanvas *somSelf, 
                                                        Environment *ev ) 
{
    ODPlatformCanvasMethodDebug("ODPlatformCanvas","ODPlatformCanvasHasWindow");

    return kODFalse;
}



SOM_Scope ODBoolean  SOMLINK ODPlatformCanvasIsInitialized( ODPlatformCanvas *somSelf, 
                                                            Environment *ev ) 
{
    ODPlatformCanvasMethodDebug("ODPlatformCanvas","ODPlatformCanvasIsInitialized");

    return kODFalse;
}





