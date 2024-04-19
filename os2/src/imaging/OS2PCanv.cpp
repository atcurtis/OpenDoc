/* @(#)Z 1.3 os2/src/imaging/OS2PCanv.cpp, odimaging, od96os2, odos29646d 96/11/15 15:50:31 (96/08/23 01:41:51) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odimaging
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

/********************************************************************/
/*                                                                  */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/********************************************************************/

#ifndef SOM_Module_os2pcanv_Source
#define SOM_Module_os2pcanv_Source
#endif
#define ODOS2Canvas_Class_Source
#define VARIABLE_MACROS

#ifndef SOM_ODPlatformCanvas_xih
#include "platcanv.xih"
#endif

#ifndef SOM_ODOS2Canvas_xih
#include "os2pcanv.xih"
#endif

#ifndef SOM_ODOS2StandardCanvas_xih
#include "os2scanv.xih"
#endif

#ifndef SOM_ODOS2WindowCanvas_xih
#include "os2wcanv.xih"
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

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif 



SOM_Scope ODPlatformCanvas*  SOMLINK ODOS2CanvasCreateConcreteCanvas( ODOS2Canvas* somSelf,
                                                               Environment *ev,
                                                               ODGraphicsSystem aSystemType,
                                                               ODBoolean isDynamic,
                                                               ODBoolean isOffScreen )
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasCreateConcreteCanvas");

    ODOS2Canvas *c = kODNULL;  ODVolatile( c );

    SOM_TRY

    if (aSystemType == kODPM)  {

       // if an on-screen, dynamic platform canvas is requested, create a 
       // "Window" platform canvas.  Else, assume the platform canvas to be 
       // created will be the "standard" platform canvas.  The "standard"
       // platform canvas represents all other combinations of isDynamic and isOffScreen.

       if ( (isDynamic == kODTrue) && (isOffScreen == kODFalse) ) 
          c = new ODOS2WindowCanvas;
       else
          c = new ODOS2StandardCanvas;
        
       THROW_IF_NULL( c );
    
       return c;
    }
    else {
       THROW( kODErrInvalidGraphicsSystem );
    }

    SOM_CATCH_ALL{
       if( c )
          ODDeleteObject( c );
       return kODNULL;
    }
    SOM_ENDTRY
}



SOM_Scope ODGraphicsSystem  SOMLINK ODOS2CanvasGetSystemType( ODOS2Canvas *somSelf, 
                                                                Environment *ev ) 
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasGetSystemType");

    return kODPM;
}



SOM_Scope ODPlatformCanvasType  SOMLINK ODOS2CanvasGetPlatformCanvasType( ODOS2Canvas *somSelf, 
                                                          Environment *ev ) 
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasGetPlatformCanvasType");

    return kODNoPlatformCanvas;
}



SOM_Scope ODBoolean  SOMLINK ODOS2CanvasHasWindow( ODOS2Canvas *somSelf, 
                                                     Environment *ev ) 
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasHasWindow");

    return kODFalse;
}



SOM_Scope ODBoolean  SOMLINK ODOS2CanvasIsInitialized( ODOS2Canvas *somSelf, 
                                                         Environment *ev ) 
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasIsInitialized");

    return kODFalse;
}



SOM_Scope void  SOMLINK ODOS2CanvasInitPlatformCanvas( ODOS2Canvas* somSelf,
                                                         Environment *ev,
                                                         HPS hps )
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasInitPlatformCanvas");

    ODSetSOMException( ev, kODErrInvalidObjectType );
}



SOM_Scope void  SOMLINK ODOS2CanvasInitPlatformWindowCanvas(ODOS2Canvas* somSelf,
                                                              Environment *ev,
                                                              HWND hwnd)
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasInitPlatformWindowCanvas");

    ODSetSOMException( ev, kODErrInvalidObjectType );
}



SOM_Scope HPS  SOMLINK ODOS2CanvasGetPS( ODOS2Canvas* somSelf,
                                           Environment *ev )
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasGetPS");

    ODSetSOMException( ev, kODErrInvalidObjectType );
    return kODNULL;

}



SOM_Scope void  SOMLINK ODOS2CanvasSetPS( ODOS2Canvas* somSelf,
                                            Environment *ev,
                                            HPS hps )
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasSetPS");

    ODSetSOMException( ev, kODErrInvalidObjectType );
}




SOM_Scope void  SOMLINK ODOS2CanvasReleasePS( ODOS2Canvas* somSelf,
                                                Environment *ev )
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasReleasePS");

    ODSetSOMException( ev, kODErrInvalidObjectType );
}



SOM_Scope HWND SOMLINK ODOS2CanvasGetWindow( ODOS2Canvas *somSelf, 
                                               Environment *ev ) 
{
    ODOS2CanvasMethodDebug("ODOS2Canvas","ODOS2CanvasGetWindow");

    ODSetSOMException( ev, kODErrInvalidObjectType );
    return kODNULL;
}




