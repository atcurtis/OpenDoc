/* @(#)Z 1.2 os2/src/imaging/OS2SCanv.cpp, odimaging, od96os2, odos29646d 96/11/15 15:50:31 (96/08/23 01:41:53) */
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
/*                                                                  */
/********************************************************************/

#ifndef SOM_Module_os2scanv_Source
#define SOM_Module_os2scanv_Source
#endif
#define ODOS2StandardCanvas_Class_Source
#define VARIABLE_MACROS



#ifndef SOM_ODOS2StandardCanvas_xih
#include "os2scanv.xih"
#endif

#define INCL_PM
#define INCL_GPI
#define INCL_GPIERRORS

#include <os2.h>
#include <pmerr.h>


#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _ERRORDEF_
#include "ErrorDef.xh"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif



SOM_Scope void  SOMLINK ODOS2StandardCanvasInitPlatformCanvas( ODOS2StandardCanvas* somSelf,
                                                         Environment *ev,
                                                         HPS hps )
{
    ODOS2StandardCanvasData *somThis = ODOS2StandardCanvasGetData(somSelf);
    ODOS2StandardCanvasMethodDebug("ODOS2StandardCanvas","ODOS2StandardCanvasInitPlatformCanvas");

    if ( !_fInitialized ) 
    {
       SOM_TRY

          ASSERT_NOT_NULL( hps );

          SIZEL size;
          HAB hab = WinQueryAnchorBlock( HWND_DESKTOP );
          WinGetLastError( hab );

          GpiQueryPS( hps, &size );
          ULONG errCode = WinGetLastError( hab );
          errCode &= 0xFFFF;
          if( errCode == PMERR_INV_HPS || errCode == PMERR_INV_HDC ) 
             THROW( kODErrInvalidParameter );

          somSelf->InitObject( ev );
          _fHps = hps;
          _fInitialized = kODTrue;

       SOM_CATCH_ALL
       SOM_ENDTRY
    }                 
}



SOM_Scope ODPlatformCanvasType  SOMLINK ODOS2StandardCanvasGetPlatformCanvasType( ODOS2StandardCanvas *somSelf, 
                                                          Environment *ev ) 
{
    ODOS2StandardCanvasMethodDebug("ODOS2StandardCanvas","ODOS2StandardCanvasGetPlatformCanvasType");

    return kODPlatformStandardCanvas;
}



SOM_Scope HPS  SOMLINK ODOS2StandardCanvasGetPS( ODOS2StandardCanvas* somSelf,
                                                   Environment *ev )
{
    ODOS2StandardCanvasData *somThis = ODOS2StandardCanvasGetData(somSelf);
    ODOS2StandardCanvasMethodDebug("ODOS2StandardCanvas","ODOS2StandardCanvasGetPS");

    if ( !_fInitialized ) {
       ODSetSOMException( ev, kODErrObjectNotInitialized );
       return kODNULL;
    }
    else
       return _fHps;
}



SOM_Scope void  SOMLINK ODOS2StandardCanvasSetPS( ODOS2StandardCanvas* somSelf,
                                            Environment *ev,
                                            HPS hps )
{
    ODOS2StandardCanvasData *somThis = ODOS2StandardCanvasGetData(somSelf);
    ODOS2StandardCanvasMethodDebug("ODOS2StandardCanvas","ODOS2StandardCanvasSetPS");

    ODSetSOMException( ev, kODErrInvalidObjectType );
}



SOM_Scope void  SOMLINK ODOS2StandardCanvasReleasePS( ODOS2StandardCanvas* somSelf,
                                                        Environment *ev )
{
    /* ODOS2StandardCanvasData *somThis = ODOS2StandardCanvasGetData(somSelf);  */
    ODOS2StandardCanvasMethodDebug("ODOS2StandardCanvas","ODOS2StandardCanvasReleasePS");

}





SOM_Scope ODBoolean  SOMLINK ODOS2StandardCanvasHasWindow( ODOS2StandardCanvas *somSelf, 
                                                             Environment *ev ) 
{
    ODOS2StandardCanvasData *somThis = ODOS2StandardCanvasGetData(somSelf);
    ODOS2StandardCanvasMethodDebug("ODOS2StandardCanvas","ODOS2StandardCanvasHasWindow");

    return kODFalse;
}


SOM_Scope HWND SOMLINK ODOS2StandardCanvasGetWindow( ODOS2StandardCanvas *somSelf, 
                                                       Environment *ev ) 
{
    ODOS2StandardCanvasData *somThis = ODOS2StandardCanvasGetData(somSelf);
    ODOS2StandardCanvasMethodDebug("ODOS2StandardCanvas","ODOS2StandardCanvasGetWindow");

    ODSetSOMException( ev, kODErrInvalidObjectType );
    return kODNULL;
}


SOM_Scope ODBoolean  SOMLINK ODOS2StandardCanvasIsInitialized( ODOS2StandardCanvas *somSelf, 
                                                                 Environment *ev ) 
{
    ODOS2StandardCanvasData *somThis = ODOS2StandardCanvasGetData(somSelf);
    ODOS2StandardCanvasMethodDebug("ODOS2StandardCanvas","ODOS2StandardCanvasIsInitialized");

    return _fInitialized;
}



SOM_Scope void  SOMLINK ODOS2StandardCanvasInitPlatformWindowCanvas( ODOS2StandardCanvas* somSelf,
                                                                       Environment *ev,
                                                                       HWND hwnd )
{
    ODOS2StandardCanvasData *somThis = ODOS2StandardCanvasGetData(somSelf);
    ODOS2StandardCanvasMethodDebug("ODOS2StandardCanvas","ODOS2StandardCanvasInitPlatformWindowCanvas");

    ODSetSOMException( ev, kODErrInvalidObjectType );
}


