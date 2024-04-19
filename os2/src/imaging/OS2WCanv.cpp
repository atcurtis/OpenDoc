/* @(#)Z 1.2 os2/src/imaging/OS2WCanv.cpp, odimaging, od96os2, odos29646d 96/11/15 15:50:32 (96/08/23 01:41:57) */
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

#ifndef SOM_Module_os2wcanv_Source
#define SOM_Module_os2wcanv_Source
#endif
#define ODOS2WindowCanvas_Class_Source
#define VARIABLE_MACROS



#ifndef SOM_ODOS2WindowCanvas_xih
#include "os2wcanv.xih"
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _ERRORDEF_
#include "ErrorDef.xh"
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif





SOM_Scope void  SOMLINK ODOS2WindowCanvasInitPlatformWindowCanvas(ODOS2WindowCanvas* somSelf,
                                                                    Environment *ev,
                                                                    HWND hwnd)
{
    ODOS2WindowCanvasData *somThis = ODOS2WindowCanvasGetData(somSelf);
    ODOS2WindowCanvasMethodDebug("ODOS2WindowCanvas","ODOS2WindowCanvasInitPlatformWindowCanvas");

    SOM_TRY
       if ( !hwnd ) 
          THROW( kODErrIllegalNullInput );  

       if ( !_fInitialized ) {
          somSelf->InitObject( ev );

          // Check if hwnd is valid.  Note: we might be able to just test 
          // if the anchor-block handle is non-null.  The WinIsWindow
          // call may not be necessary.

          // Get handle to anchor-block.
          HAB hab;
          if( (hab = WinQueryAnchorBlock( hwnd )) != NULLHANDLE ) {
             if( WinIsWindow( hab, hwnd ) )  {
                _fHWND = hwnd;
                _fInitialized = kODTrue;
             }
             else {
                ODSetSOMException( ev, kODErrInvalidParameter );
             }   
          }
          else
             ODSetSOMException( ev, kODErrInvalidParameter );
       }
    SOM_CATCH_ALL
    SOM_ENDTRY

}



SOM_Scope ODPlatformCanvasType  SOMLINK ODOS2WindowCanvasGetPlatformCanvasType( ODOS2WindowCanvas *somSelf, 
                                                          Environment *ev ) 
{
    ODOS2WindowCanvasMethodDebug("ODOS2WindowCanvas","ODOS2WindowCanvasGetPlatformCanvasType");

    return kODPlatformWindowCanvas;
}



SOM_Scope void  SOMLINK ODOS2WindowCanvasInitPlatformCanvas( ODOS2WindowCanvas* somSelf,
                                                               Environment *ev,
                                                               HPS hps )
{
    ODOS2WindowCanvasData *somThis = ODOS2WindowCanvasGetData(somSelf);
    ODOS2WindowCanvasMethodDebug("ODOS2WindowCanvas","ODOS2WindowCanvasInitPlatformCanvas");

    ODSetSOMException( ev, kODErrInvalidObjectType );
}



SOM_Scope HPS  SOMLINK ODOS2WindowCanvasGetPS( ODOS2WindowCanvas* somSelf,
                                                 Environment *ev )
{
    ODOS2WindowCanvasData *somThis = ODOS2WindowCanvasGetData(somSelf);
    ODOS2WindowCanvasMethodDebug("ODOS2WindowCanvas","ODOS2WindowCanvasGetPS");

    if ( !_fInitialized ) {
       ODSetSOMException( ev, kODErrObjectNotInitialized );
       return kODNULL;
    }
    else {
       if ( _fHps ) 
          _fCachedPSRefCount++;
       else {
          _fHps = WinGetPS( _fHWND );
          _fCachedPSRefCount = 1;
       }
       return _fHps;
    }
}


SOM_Scope void  SOMLINK ODOS2WindowCanvasSetPS( ODOS2WindowCanvas* somSelf,
                                                  Environment *ev,
                                                  HPS hps)
{
    ODOS2WindowCanvasData *somThis = ODOS2WindowCanvasGetData(somSelf);
    ODOS2WindowCanvasMethodDebug("ODOS2WindowCanvas","ODOS2WindowCanvasSetPS");

    if ( !_fInitialized ) {
       ODSetSOMException( ev, kODErrObjectNotInitialized );
    }
    else {
       if ( !_fCachedPSRefCount )  {
          if ( hps )  {
             /*
              * Make sure the PS is associated with the window DC for the
              *  facet window.
              */
             if( WinQueryWindowDC( _fHWND ) == GpiQueryDevice( hps ) )
                 _fHps = hps;
             else
                ODSetSOMException( ev, kODErrInvalidParameter );
          }
          else
             _fHps = 0;
       }
       else
          ODSetSOMException( ev, kODErrRefCountGreaterThanZero );
    }
}




SOM_Scope void  SOMLINK ODOS2WindowCanvasReleasePS( ODOS2WindowCanvas* somSelf,
                                               Environment *ev )

{
    ODOS2WindowCanvasData *somThis = ODOS2WindowCanvasGetData(somSelf);  
    ODOS2WindowCanvasMethodDebug("ODOS2WindowCanvas","ODOS2WindowCanvasReleasePS");

    if ( !_fInitialized )
       ODSetSOMException( ev, kODErrObjectNotInitialized );
    else {
       if ( _fCachedPSRefCount ) {
          if ( --_fCachedPSRefCount == 0 ) {
             WinReleasePS( _fHps );
             _fHps = 0;
          }
       }
    } 
}



SOM_Scope ODBoolean  SOMLINK ODOS2WindowCanvasHasWindow( ODOS2WindowCanvas *somSelf, 
                                                           Environment *ev ) 
{
    ODOS2WindowCanvasData *somThis = ODOS2WindowCanvasGetData(somSelf);
    ODOS2WindowCanvasMethodDebug("ODOS2WindowCanvas","ODOS2WindowCanvasHasWindow");

    if ( !_fInitialized ) {
       ODSetSOMException( ev, kODErrObjectNotInitialized );
       return kODFalse;
    }
    else
       return kODTrue;
}


SOM_Scope HWND  SOMLINK ODOS2WindowCanvasGetWindow( ODOS2WindowCanvas *somSelf, 
                                                           Environment *ev ) 
{
    ODOS2WindowCanvasData *somThis = ODOS2WindowCanvasGetData(somSelf);
    ODOS2WindowCanvasMethodDebug("ODOS2WindowCanvas","ODOS2WindowCanvasGetWindow");

    if ( !_fInitialized ) {
       ODSetSOMException( ev, kODErrObjectNotInitialized );
       return kODNULL;
    }
    else
       return _fHWND;
}


SOM_Scope ODBoolean  SOMLINK ODOS2WindowCanvasIsInitialized( ODOS2WindowCanvas *somSelf, 
                                                               Environment *ev ) 
{
    ODOS2WindowCanvasData *somThis = ODOS2WindowCanvasGetData(somSelf);
    ODOS2WindowCanvasMethodDebug("ODOS2WindowCanvas","ODOS2WindowCanvasIsInitialized");

    return _fInitialized;
}



