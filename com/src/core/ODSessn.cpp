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
/* @(#) 1.29 com/src/core/ODSessn.cpp, odcore, od96os2, odos29646d 11/12/96 12:21:27 [11/15/96 15:24:47] */
/*
        File:           ODSessn.cpp

        Contains:       ODSession class implementation

        Owned by:       Nick Pilch

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                <50>     10/8/95        TJ              Fixes Recomended by Refball
                <49>     4/10/95        NP              1287722: Change signature of
                                                                        ODSession::GetUserName.
                <48>    25/09/95        NP              1280561: Check results of operator new.
                <47>     8/29/95        DM              1276569: FB3:ODSession::GetUserName can
                                                                        return bogus string
                <46>     8/29/95        RR              # 1276401 Register Mouse Focus
                <45>     8/16/95        NP              1242634: Remove #define PLATFORMFILEVIRTUAL
                                                                        when PlatformFile has no virt. methods.
                <44>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                <43>     7/27/95        CC              1153788: Use std C library routines rand
                                                                        and srand for generating unique update ids.
                <42>     5/30/95        CG              #1252280 BB: PStrToIntl should not  be used
                                                                        by OpenDoc.
                <41>     5/26/95        DM              1251403: Multithreading naming support.
                <40>     5/24/95        CG              #1239535 BB: Added temporary work around.
                <39>     5/11/95        NP              1242421: Fix exception handling
                <38>     5/11/95        VL              1238421: Implemented Purge.
                <37>     4/26/95        CG              1211082 BB: 5$ Bugs need to be evaluated
                                                                        and removed from Core
                <36>      4/5/95        CG              #1216169 BB, #1221135: When Editors folder
                                                                        or OpenDoc Libraries folder is missing,
                                                                        OpenDoc quits and displays error.
                <35>     3/27/95        CC              1231902: Pass ODSession to
                                                                        InitTranslation().
                <34>      3/7/95        RR              #1220929 Caching of foci in ODWindowState
                                                                        necessitated reordering of Inits
                <33>     2/16/95        CG              #1207793: defined resource id constant.
                <32>     2/10/95        CG              #1177475: Need to initialize translation
                                                                        object before nameSpaceManager.
                <31>    12/13/94        SS              1205663 - changed ODQD to gODQD
                <30>     12/5/94        jpa             Added global ODQD & initialization code
                                                                        [1203926]
                <29>    11/30/94        RR              Include time.h
                <28>    11/15/94        NP              1200193-The Shell should instantiate its
                                                                        Semantic Interface.
                <27>    11/15/94        NP              1200132-Removed instantiation of Shell
                                                                        Semantic Interface from here.
                <26>     10/7/94        CC              1191489 - Moved UniqueUpdateID() in from
                                                                        base class.
                <25>      9/9/94        NP              1184043: Change to InitSemanticInterface.
                <24>     8/22/94        T‚              #1182448 System Process quits when no
                                                                        OpenDoc documents are running
                <23>     8/19/94        NP              1181622: Ownership fix.
                <22>     8/13/94        T‚              #1180814 Added support for ODPartWrapper
                                                                        API and removed a chr(16) on line #288
                                                                        which was randomly introduced by projector?
                <21>     8/12/94        NP              1180762-added setters for getters. moved
                                                                        stuff to B file. Removed GetSemtInterface.
                <20>      8/3/94        CC              Register the clipboard focus.
                <19>      8/3/94        VL              1153123: Storage to ODStor.
                <18>      8/2/94        T‚              bug# for #17 right below was #1178167
                <17>      8/2/94        T‚              added TOKENTABLECORRUPTED to work around
                                                                        heap corruption occuring when fTokenTable
                                                                        was deleted.  Changed deletes to
                                                                        ODDeleteObject.
                <16>     7/29/94        CG              Removed Object registration APIs from
                                                                        session.
                <15>     7/21/94        T‚              explicitly create & delete a PlatformFile
                                                                        to make sure it is not deadstripped, added
                                                                        #define PLATFORMFILEVIRTUAL
                <14>     7/11/94        T‚              added GetInfo, SetInfo, set _fInfo in
                                                                        InitSession
                <13>     7/11/94        NP              Fixed compile problem in GetUserName.
                <12>      7/8/94        NP              Fix GetUserName.
                <11>      7/7/94        RR              Initialize dispatcher AFTER token table
                <10>      7/1/94        NP              Change to InitSIHelper.
                 <9>      7/1/94        CG              Uncommented code in GetUserName.
                 <8>     6/30/94        CG              Uncommented messaging-related fields and
                                                                        methods, moved StdDisp initialization to
                                                                        Dispatch.cpp.
                 <7>     6/28/94        VL              Use DragDrp.
                 <6>     6/28/94        VL              Fixed parameters to InitStorageSystem.
                 <5>     6/24/94        CG              Initialization clean up.
                 <4>     6/23/94        CG              Somverted original implementation.
                 <3>     6/19/94        MB              Implement GetDefaultHeap
                 <2>     6/18/94        MB              Add memory component
                 <1>     6/14/94        CG              first checked in
                ------------------------------------------------------------
                 <7>      6/6/94        jpa             Eliminate spurious ASLM header include.
                                                                        [1153437]
                 <6>      6/1/94        CG              Moved fPreferences to nameSpaceManager and
                                                                        changed session to use public binding
                                                                        object.
                 <5>     5/27/94        CG              #1165186: Abstracted prefs out of binding.
                 <4>      5/9/94        MB              #1162181: Changes necessary to install MMM.
                 <3>      4/1/94        T‚              #1154941: Get OpenDoc source to actually
                                                                        compile for PowerPC
                 <2>     3/31/94        eeh             bug #1153999: PStrToIntl interface changed.
                 <1>     3/28/94        CG              first checked in
                 <6>     3/26/94        NP              1153512. Object Registration API needed.
                 <5>     3/26/94        NP              1153504. InitSemanticInterface.
                 <4>     3/25/94        CC              Change name of included file to
                                                                        <LinkMgr.xh>. (1153348)
                 <3>     3/15/94        MB              Changes to support SCpp/ASLM builds,
                                                                        #1150864.
                 <2>     2/28/94        CG              (no bug #) Changed call to GetSystemPool()
                                                                        to XMPGetSystemHeap().
                <39>      2/9/94        T‚              Throw to THROW
                <38>      2/7/94        T‚              more PPC header fixes
                <37>      2/4/94        T‚              fixes to compile with PPC Universal headers
                <36>      2/3/94        CG              System Session gets created in InitSession.
                <35>      2/1/94        RR              Call InitSessionForPlatform in InitSession
                <34>     1/28/94        CC              Added UniqueUpdateID().
                <33>     1/15/94        RR              WinState.h->WinStat.h,
                                                                        Dispatch.h->Disptch.h
                <32>     1/14/94        NP              Init changes.
                <31>     1/14/94        CG              Added include for new NmSpcMgr.h.
                <30>     1/12/94        CG              Added fBinding field and calls to new
                                                                        ODBinding and InitBinding.
                <29>     1/12/94        VL              Init changes.
                <28>    12/20/93        RR              New constructor/init interface
                <27>    12/15/93        T‚              more InitObject changes, rename
                                                                        Initialize() method call for ClipBoard and
                                                                        NameSpaceManager
                <26>     12/8/93        CC              Create and Initialize Link Manager object
                <25>     12/3/93        T‚              Stop including ODError.h, it is included
                                                                        as ErrorDef.h inside Except.h
                <24>     12/2/93        CC              Call InitClipboard().
                <23>    11/16/93        RR              Register stddispatchmodule for
                                                                        kHighLevelEvent
                <22>    11/15/93        NP              Changed XMPSymbols to ODNameSpaceManager.
                <21>    10/28/93        NP              GetType returns a boolean instead of
                                                                        throwing an exception.
                <20>    10/22/93        NP              Added session parameter to ODNameResolver
                                                                        constructor.
                <19>    10/20/93        CC              Added session parameter to ODClipboard
                                                                        constructor
                <18>     9/20/93        NP              Changed order of ODMessageInterface and
                                                                        ODNameResolver initialization.
                <17>     8/11/93        RR              Added code to install standard dispatch
                                                                        module (moved from dispatcher)
                <16>     7/28/93        NP              Added GetSession function.
                <15>     7/21/93        T‚              change destructor to delete fTokenTable
                                                                        before deleting fNameSpaceManager (~TokenTable
                                                                        seemed to need fNameSpaceManager)
                <14>     7/12/93        CG              Added fShellSemtInterface and
                                                                        AcquireShellSemtInterface() to Session class.
                <13>     6/23/93        RCR             Initialize TokenTable before arbitrator.
                <12>     6/22/93        NP              Added Tokenize methods.
                <11>     5/18/93        NP              Changed call to ODMessageInterface
                                                                        constructor.
                <10>     4/28/93        NP              File name changes.
                 <9>     4/27/93        NP              Uncommented last comments.
                 <8>     4/27/93        NP              Commented out the construction of some
                                                                        objects. Fixed OpenODSession.
                 <7>     4/23/93        NP              Removed warning from Purge.
                 <6>     4/23/93        NP              Fixed GetStorageSystem.
                 <5>     4/23/93        NP              Fixed Purge.
                 <4>     4/23/93        NP              Fixed call to ODSemanticInterface
                                                                        constructor.
                 <3>     4/23/93        NP              Fixed Initialize to call Initialize members
                                                                        for objects it creates. Uncommented
                                                                        constructor and zeroed out fields in it.
                 <2>     4/22/93        RCR             Add Session:: to Initialize. Remove ODMethod
                 <1>     4/21/93        NP              first checked in

        To Do: 1276569: when name is not found return empty string from GetUserName()
*/


#define INCL_DOSPROCESS
#define INCL_WIN
#define INCL_WPCLASS
#define INCL_WINERRORS
#define SOM2VERSION
#include <os2.h>

#ifndef _ALTPOINT_
#include "AltPoint.h"                   // Use C++ savvy ODPoint and ODRect
#endif

#define ODSession_Class_Source
#define VARIABLE_MACROS
#include <ODSessn.xih>

// headers needed for DSOM linking

#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <somd.xh>
#include <somoa.xh>
#include <implrep.xh>
#include <impldef.xh>

#include <IODDefs.xh>
#include <ilnksrvc.xh>
#include <avsshell.h>
#include <iavlsvr.xh>
#include <iciditr.xh>
#include <ilinksrc.xh>
#include <ilnktrgt.xh>

//dqOSA Start
//dqOSA These are for the OD Semantic Interface
// #ifndef SOM_ODDesc_xh
// #include "ODDesc.xh"
// #endif

// #ifndef SOM_ODOSAEvent_xh
// #include "ODOSAEvt.xh"
// #endif

// #ifndef SOM_ODOSLToken_xh
// #include "ODOSLTkn.xh"
// #endif

// #ifndef __ODDESUTL__
// #include "ODDesUtl.h"
// #endif

// #ifndef SOM_ODObjectSpec_xh
// #include "ODObjSpc.xh"
// #endif

// #ifndef SOM_ODSemanticInterface_xh
// #include <semtintf.xh>
// #endif

//dqOSA This is for the OD Name Resolver
//#ifndef SOM_ODNameResolver_xh
//#include <namrslvr.xh>
//#endif


//dqOSA This is for the OD Message Interface
//#ifndef SOM_ODMessageInterface_xh
//#include <mssgintf.xh>
//#endif
//dqOSA end

//#ifndef SOM_ODPerWindowFocusModule_xh
//#include <winfocus.xh>
//#endif



#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef SOM_ODStorageSystem_xh
#include <ODStor.xh>
#endif

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODDispatcher_xh
#include <Disptch.xh>
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_ODInfo_xh
#include <Info.xh>
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef SOM_ODClipboard_xh
#include <Clipbd.xh>
#endif

#ifndef SOM_ODDragAndDrop_xh
#include <DragDrp.xh>
#endif

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_) || defined(_PLATFORM_MACINTOSH_)
#ifndef SOM_ODLinkManager_xh
#include <LinkMgr.xh>
#endif
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef SOM_ODUndo_xh
#include <Undo.xh>
#endif

#ifdef _TOOLSPACE_
#ifndef SOM_ODToolSpaceManager_xh
#include <ODTlSpc.xh>
#endif
#endif

#ifndef SOM_ODBinding_xh
#include <ODBindng.xh>
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include <Foci.xh>
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif
#endif

#ifndef _TOKNTABL_
#include "ToknTabl.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __RESOURCES__
#include <Resources.h>
#endif
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __TEXTUTILS__
#include <TextUtils.h>
#endif
#endif

#ifndef __TIME_H__
#include <time.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __SCRIPT__
#include <script.h>
#endif
#endif

#if (defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_))
#ifndef SOM_ODHelp_xh
#include <ODHelp.xh>
#endif
#endif //_PLATFORM_WIN32_ || _PLATFORM_OS2_

#ifndef __STDLIB__
#include <stdlib.h>
#endif

#ifdef _PLATFORM_WIN32_
#include <olemgrw.h>
#endif

#define ODSession_Class_Source
#define VARIABLE_MACROS
#include <ODSessn.xih>

void CommonInitialization(ODSession *somSelf, Environment *ev, ODSessionData *somThis);

#pragma segment ODSession

#define kCF_OPENDOCDOCUMENT "CF_OPENDOCDOCUMENT"
#define CF_OPENDOCDOCUMENT  WinFindAtom(WinQuerySystemAtomTable(),kCF_OPENDOCDOCUMENT)
#define kCF_OPENDOCOWNERID "CF_OPENDOCOWNERID"
#define CF_OPENDOCOWNERID   WinFindAtom(WinQuerySystemAtomTable(),kCF_OPENDOCOWNERID)

//==============================================================================
// ODSession
//==============================================================================

//------------------------------------------------------------------------------
// ODSession: InitSession
//------------------------------------------------------------------------------
SOM_Scope void   SOMLINK ODSessionInitSession(ODSession *somSelf,
                                              Environment *ev)
{
    ODSessionData *somThis = ODSessionGetData(somSelf);

    ODSessionMethodDebug("ODSession","InitSession");

    try {

//    if (somSelf->IsInitialized(ev))
//      return;

    somSelf->InitBaseSession(ev);

    _fState = ODSession_DOC_SESSION;

    CommonInitialization(somSelf, ev, somThis);

     }       
     catch (ODException _exception) {
          SetErrorCode(kODNoError);
     }

}

SOM_Scope void   SOMLINK ODSessionInitAvailServerSession(ODSession *somSelf, Environment *ev, string alias)
{
    ODSessionData *somThis = ODSessionGetData(somSelf);
    ODSessionMethodDebug("ODSession","InitAvailServerSession");

    // check to see if linking is on or off (131199)
    char* linkonoff = getenv("ODLINKING");

    if(linkonoff !=NULL )
    {
      if( strcmpi( linkonoff, "OFF"))
      {
         _fLinkONOFFflag = 1;
      }
      else
      {
         _fLinkONOFFflag = 0;
      }
    }
    else
    {
       _fLinkONOFFflag = 1;
    }

//    if (somSelf->IsInitialized(ev))
//    return;

    somSelf->InitBaseSession(ev);
    _fState = ODSession_AS_SESSION;

  CommonInitialization(somSelf, ev, somThis);
}

void CommonInitialization(ODSession *somSelf, Environment *ev, ODSessionData *somThis)
{
    _fMenuFocusModule = kODNULL;
//    _fState           = ODSession_UNINIT;
    time_t theTime;
    time(&theTime);
    srand(theTime);
    // THIS TEST comes from Startup :
    char* linkonoff = getenv("ODLINKING");

    if(linkonoff !=NULL )
    {
      if(strcmpi( linkonoff, "OFF"))
      {
        _fLinkONOFFflag = 1;
      }
      else
      {
        _fLinkONOFFflag = 0;
      }
    }
    else
    {
      _fLinkONOFFflag = 1;
    }
      //TMW
    somSelf->SetStorageSystem(ev, new ODStorageSystem);
    somSelf->GetStorageSystem(ev)->InitStorageSystem(ev, somSelf);

    if (_fState != ODSession_AS_SESSION)
    {

     WinAddAtom( WinQuerySystemAtomTable(), kCF_OPENDOCDOCUMENT ); //<crs>
     WinAddAtom( WinQuerySystemAtomTable(), kCF_OPENDOCOWNERID );  //<crs>

     somSelf->SetClipboard(ev, new ODClipboard);
     somSelf->GetClipboard(ev)->InitClipboard(ev, somSelf);
     somSelf->SetInfo(ev, new ODInfo);
     somSelf->GetInfo(ev)->InitInfo(ev, somSelf);
     somSelf->SetTranslation(ev, new ODTranslation); //[132012]
     somSelf->GetTranslation(ev)->InitTranslation(ev, somSelf);
    }
    somSelf->SetNameSpaceManager(ev, new ODNameSpaceManager);
    somSelf->GetNameSpaceManager(ev)->InitNameSpaceManager(ev, somSelf);
    somSelf->SetBinding(ev, new ODBinding);
    somSelf->GetBinding(ev)->InitBinding(ev, somSelf);

    if (_fState != ODSession_AS_SESSION)
    {
//    somSelf->SetNameResolver(ev, new ODNameResolver);
//    somSelf->GetNameResolver(ev)->InitNameResolver(ev, somSelf);
//    somSelf->SetMessageInterface(ev, new ODMessageInterface);
//    somSelf->GetMessageInterface(ev)->InitMessageInterface(ev, somSelf);
//    somSelf->SetShellSemtInterface(ev, new ODSemanticInterface);
//    somSelf->AcquireShellSemtInterface(ev)->InitSemanticInterface(ev,kODAppShell, somSelf);

      somSelf->SetUndo(ev, new ODUndo);
      somSelf->GetUndo(ev)->InitUndo(ev);

      somSelf->SetTokenTable(ev, new TokenTable(somSelf->GetNameSpaceManager(ev)));
      somSelf->GetTokenTable(ev)->Initialize();

      somSelf->SetWindowState(ev, new ODWindowState); //[120604] Must be after token table
      somSelf->GetWindowState(ev)->InitWindowState(ev, somSelf);
      somSelf->SetDragAndDrop(ev, new ODDragAndDrop); //[120604] Must be after window state
      somSelf->GetDragAndDrop(ev)->InitDragAndDrop(ev, somSelf);
      somSelf->SetArbitrator(ev, new ODArbitrator); // Must be after token table
      somSelf->GetArbitrator(ev)->InitArbitrator(ev, somSelf);

    }
//   if (_fState != ODSession_AS_SESSION)
//    {
//      _fMenuFocusModule = new ODPerWindowFocusModule();
//      _fMenuFocusModule->InitPerWindowFocusModule(ev, somSelf);
//    }

   if(_fLinkONOFFflag)
    {
      _fLinkService = new IODLinkService();
    }

   if (_fState != ODSession_AS_SESSION)
    {
      somSelf->SetDispatcher(ev, new ODDispatcher); // Must be after token table
      somSelf->GetDispatcher(ev)->InitDispatcher(ev, somSelf);
    }

#ifdef PLATFORMFILEVIRTUAL
// $$$$$ to make sure PlatformFile code is not deadstripped.
  // NP: Can't you wrap this in a if(0) so that it doesn't get actually
  //  executed?
  PlatformFile* file = new PlatformFile;
  ODDeleteObject(file);
#endif

   if (_fState != ODSession_AS_SESSION)
    {
#ifdef _TOOLSPACE_
      // Added for Toolspace support
      ODToolSpaceManager* toolspacemgr = new ODToolSpaceManager;
      toolspacemgr->InitToolSpaceManager(ev, somSelf);
      somSelf->SetToolSpaceManager(ev, toolspacemgr);
#endif

      // temporary work around for #1239535
      somSelf->GetNameSpaceManager(ev)->SynchNSTable(ev);  // Must be after dispatcher

      ODArbitrator* arbitrator = somSelf->GetArbitrator(ev);

      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODKeyFocus),kODNULL);
      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODMenuFocus),kODNULL);
      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODSelectionFocus),kODNULL);
      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODModalFocus),kODNULL);

      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODScrollingFocus),kODNULL);
      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODClipboardFocus),kODNULL);
      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODMouseFocus),kODNULL);

        // toolspace foci
      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODDeskTopToolFocus),kODNULL);
      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODDocumentToolFocus),kODNULL);
      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODRootToolFocus),kODNULL);
      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODPartFrameToolFocus),kODNULL);
//      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODStatusToolFocus),kODNULL);
//      arbitrator->RegisterFocus(ev,somSelf->Tokenize(ev,kODStatusTextFocus),kODNULL);
  } /* end if ODSession != AS */

  if(_fLinkONOFFflag)
  {
     IODAvailServerNewClass(0, 0);
     IODLinkSourceNewClass(0, 0);
     IODLinkTargetNewClass(0, 0);
  }

  // Initialize the ODHelp object here so that F1 support for menus is hooked in
  _fHelp = new ODHelp;
  _fHelp->InitHelp(ev, somSelf);
}



//------------------------------------------------------------------------------
// ODSession: ~ODSession
//------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODSessionsomUninit(ODSession *somSelf)
{
    ODSessionData *somThis = ODSessionGetData(somSelf);
    ODSessionMethodDebug("ODSession","somUninit");

//  delete _fMenuFocusModule;

    switch (_fState) {
      case ODSession_DOC_SESSION:
        if(_fLinkONOFFflag)
        {
           delete _fLinkService;
        }
        break;
      case ODSession_AS_SESSION:
        if(_fLinkONOFFflag)
        {
           delete _fLinkService;
        }
        break;
      default:
        break;
    }

    Environment* ev = SOM_CreateLocalEnvironment();
    try {
      if (_fHelp)
      {
        _fHelp->TerminateHelp(ev);
        _fHelp = kODNULL;
      }
     }       
     catch (ODException _exception) {
          SetErrorCode(kODNoError);
     }
    // Destroy local SOM environment we created.
    SOM_DestroyLocalEnvironment(ev);

//    ODSession_parents_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODSession: Purge
//------------------------------------------------------------------------------

SOM_Scope ODSize  SOMLINK ODSessionPurge(ODSession *somSelf, Environment *ev,
                ODSize size)
{
    ODSessionMethodDebug("ODSession","Purge");

        ODSize purged;

        try {

        somSelf->GetArbitrator(ev)->Purge(ev, size);
        somSelf->GetBinding(ev)->Purge(ev, size);
        somSelf->GetClipboard(ev)->Purge(ev, size);
        somSelf->GetDispatcher(ev)->Purge(ev, size);
        somSelf->GetDragAndDrop(ev)->Purge(ev, size);
//      somSelf->GetLinkManager(ev)->Purge(ev, size);
//      somSelf->GetMessageInterface(ev)->Purge(ev, size);
//      somSelf->GetNameResolver(ev)->Purge(ev, size);
        somSelf->GetNameSpaceManager(ev)->Purge(ev, size);
        somSelf->GetStorageSystem(ev)->Purge(ev, size);
        somSelf->GetTranslation(ev)->Purge(ev, size);
        somSelf->GetUndo(ev)->Purge(ev, size);
        somSelf->GetWindowState(ev)->Purge(ev, size);

        purged= parent_Purge(somSelf, ev, size);

     }       
     catch (ODException _exception) {
                purged = 0;
          SetErrorCode(kODNoError);
     }
     return purged;
}

//------------------------------------------------------------------------------
// ODSession: GetUserName
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODSessionGetUserName
                (ODSession *somSelf, Environment *ev,
                ODIText** name)

{
    ODSessionMethodDebug("ODSession","GetUserName");

        StringHandle    userName;

        try {
                if (!name) ODSetSOMException(ev,kODErrIllegalNullInput);

#ifdef _PLATFORM_MACINTOSH_
                //ODScriptCode script = GetScriptManagerVariable(smSysScript);
                ODScriptCode script = FontToScript(GetAppFont());
                ODLangCode lang = GetScriptVariable(script, smScriptLang);

                name->format = kODTraditionalMacText;
                name->text._buffer = kODNULL;
                SetITextBufferSize(name, sizeof(ODTradITextDataHeader), kODFalse);
                SetITextScriptCode(name, script);
                SetITextLangCode(name, lang);

                userName = GetString(kMacUserNameResID);
                if (userName)
                {
                        ODLockHandle((ODHandle)userName);
                        SetITextString(name, (StringPtr)*userName);
//                      name = CreateITextPString(script, lang, (StringPtr)(*userName));

                        ODUnlockHandle((ODHandle)userName);
                        ReleaseResource((Handle)userName);
                }
                else
//                      name = CreateITextPString(script, lang, (StringPtr) "");
#endif

                char* buffer = getenv("USERNAME");
                if (!buffer) {
                     *name = CreateITextCString(0,0,"");
                } else {
                     char* login_name = (char* ) SOMMalloc(strlen(buffer)+1);
                     strcpy(login_name, buffer);
                     *name = CreateITextFromCharPtr(login_name);
                } /* endif */

#ifdef _PLATFORM_MACINTOSH_
                *name = *name;
#endif
        }
        catch (ODException _exception) {
             SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODSession: UniqueUpdateID
//------------------------------------------------------------------------------

SOM_Scope ODUpdateID  SOMLINK ODSessionUniqueUpdateID
                (ODSession *somSelf, Environment *ev)
{
    ODSessionMethodDebug("ODSession","UniqueUpdateID");

        return (ODUpdateID) rand();
}



// this is executed on termination
void cleanup(Environment *ev, ULONG shouldRegister)
{
    if (SOMD_ImplDefObject != NULL)
    {
      // If we registered, Unregister server from implementation registry
      //if (shouldRegister)
      //   SOMD_ImplRepObject->delete_impldef
      //                            (ev, SOMD_ImplDefObject->_get_impl_id(ev));
      if (strcmp(SOMD_ImplDefObject->_get_impl_program(ev),
                 "docshell.exe") == 0)
      {
         SOMD_ImplRepObject->delete_impldef
                               (ev, SOMD_ImplDefObject->_get_impl_id(ev));
      }
      delete SOMD_ImplDefObject;
    }

    if (SOMD_SOMOAObject != NULL)
    {
      delete SOMD_SOMOAObject;
    }

    SOMD_Uninit(ev);
    SOM_UninitEnvironment(ev);
}


void  APIENTRY serverExitHandlerRegistered()
{
    Environment ev;

    SOM_InitEnvironment(&ev);

    if (SOMD_SOMOAObject)
    {
      SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);
    }

    cleanup(&ev, (ULONG)1);

    DosExitList(EXLST_EXIT, (PFNEXITLIST)serverExitHandlerRegistered);
}

void  APIENTRY serverExitHandlerUnregistered()
{
    Environment ev;

    SOM_InitEnvironment(&ev);
    SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);
    cleanup(&ev, (ULONG)0);

    DosExitList(EXLST_EXIT, (PFNEXITLIST)serverExitHandlerUnregistered);
}


void dsomThread(ULONG shouldRegister)
{
    Environment        ev;
    //ImplementationDef* implDef;
    //char               alias[64];

    PTIB ptib;    // @SESH [136778]
    PPIB ppib;    // @SESH [136778]

    // @SESH  [136778]-- Associate PM message queue to this thread
    DosGetInfoBlocks(&ptib, &ppib);
    HMQ hmqdsomThread = WinQueueFromID(0, ppib->pib_ulpid, 1);
    WinThreadAssocQueue(0, hmqdsomThread);

    // Local and DSOM initialization
    SOM_InitEnvironment(&ev);
    //SOMD_Init(&ev);

       // Register exit handler
    // We need two specialized exit handlers, since
    //   exit handlers don't seem to take parameters (!!)
//   if (shouldRegister)
//      DosExitList(EXLST_ADD, (PFNEXITLIST) serverExitHandlerRegistered);
//   else
//      DosExitList(EXLST_ADD, (PFNEXITLIST) serverExitHandlerUnregistered);
    // Process requests until deactivation
    (void) SOMD_SOMOAObject->execute_request_loop(&ev, SOMD_WAIT);
    if (ev._major == NO_EXCEPTION)
    {
      SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);
      cleanup(&ev, shouldRegister);
      DosExit(EXIT_PROCESS, 1);
    }
    else
    {

#ifdef ODDebug
      somPrintf("execute_request_loop failed, exception = %s\n",
                somExceptionId(&ev));
#endif
      SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);
      cleanup(&ev, shouldRegister);
      DosExit(EXIT_PROCESS, 0);
    }
}

void startup(Environment *ev, char *optAlias, ULONG shouldRegister, ODSessionData *somThis)
{
// Create a DSOM environment and a DSOM server thread

// register means dynamically register with the implementation registry
//   and unregister when the DSOM thread shuts down
// optAlias null means pick a random name (should include hostname)
//   otherwise means use a given name

      char   alias[256];
      TID    fDSOMThreadID;
      APIRET rc;

      if(_fLinkONOFFflag)
      {
//#if 0
         if (!WinIsSOMDDReady())
           {
             //Display - starting somdd.exe
            APIRET rc = WinRestartSOMDD(kODTrue);
            if(rc == PMERR_SOMDD_NOT_STARTED)
              {
              //Display error that somdd cannot be started
              //abort!!!!!

               WinMessageBox(HWND_DESKTOP,
                       HWND_DESKTOP,
                       "Cannot start DSOM daemon!",
                       "Error!",
                       0L,
                       MB_OK | MB_ERROR);
              DosExit(EXIT_PROCESS, 0);
              }
            int i = 0;
            while((!WinIsSOMDDReady()) && (i < 60))
              {
                DosSleep(1000);
                i++;
              }

            while((!WinIsSOMDDReady()) && (i == 60))
            {

            //Display - Unable to properly start somdd.exe
            //abort!!!!!!!
               WinMessageBox(HWND_DESKTOP,
                       HWND_DESKTOP,
                       "Cannot start DSOM daemon!",
                       "Error!",
                       0L,
                       MB_OK | MB_ERROR);
             DosExit(EXIT_PROCESS, 0);
            }
           }
//#endif

         SOMD_Init(ev);

              if (ev->_major != NO_EXCEPTION)
              {
               THROW_M(ev->_major, "Unable to start Opendoc Link Server.Please check DSOM configuration.");
#if 0
               WinMessageBox(HWND_DESKTOP,
                       HWND_DESKTOP,
                       "Cannot start DSOM daemon!",
                       "Error!",
                       0L,
                       MB_OK | MB_ERROR);
#endif
              }
         if(getenv("HOSTNAME") == NULL)
         {

#ifdef ODDebug
           somPrintf("\n Error: Incomplete DSOM environment setup: Missing HOSTNAME\n");//@SK [123273]
#endif
           DosExit(EXIT_PROCESS, 1);
         }


         if (optAlias == kODNULL)
         {
           _itoa(_getpid(), alias, 10);
           strcat(alias, getenv("HOSTNAME"));
         }
         else
         {
           strcpy (alias, optAlias);
         }



         // If requested, register server with implemenation registry
         if (shouldRegister) {
            ImplementationDef* implDef = new ImplementationDef;
            if (implDef)
            {
              implDef->_set_impl_alias(ev, alias);
              implDef->_set_impl_program(ev, "docshell.exe");
              implDef->_set_impl_server_class(ev, "SOMDServer");
//@SESH [142206]
              if(getenv("USE_OLE2") == NULL)
                implDef->_set_impl_flags(ev, IMPLDEF_MULTI_THREAD);
              else
                implDef->_set_impl_flags(ev, 0x00000000);
              implDef->_set_impl_refdata_file(ev, "");
              implDef->_set_impl_refdata_bkup(ev, "");
              if (getenv("REMOTE_LINKING_ENABLED")) {
                implDef->_set_impl_hostname(ev, getenv("HOSTNAME"));
              } else {
                implDef->_set_impl_hostname(ev, "localhost");
              } /* endif */

              SOMD_ImplRepObject->add_impldef(ev, implDef);
              if (ev->_major != NO_EXCEPTION)
              {

#ifdef ODDebug
                somPrintf("add_impldef failed, exception = %s\n", somExceptionId(ev));
#endif
                delete implDef;
                cleanup(ev, shouldRegister);
                DosExit(EXIT_PROCESS, 1);
              }
           }
           else
           {

#ifdef ODDebug
             somPrintf("Construct ImplemenationDef failed\n");
#endif
             cleanup(ev, shouldRegister);
             DosExit(EXIT_PROCESS, 1);
           }
       }
       // Get ImplementationDef from Implementation Repository
       SOMD_ImplDefObject = SOMD_ImplRepObject->
                               find_impldef_by_alias
                               (ev, alias);

       if (ev->_major != NO_EXCEPTION)
       {
        char* msg = (char*)SOMMalloc(sizeof(alias)+50);
#ifdef _UNICODE_
        WinLoadString( WinQueryAnchorBlock(HWND_DESKTOP), NULLHANDLE,
                       OD_NODEFINITION, sizeof(szMsg1), szMsg1);
        sprintf(msg,
                "%s %s!",
                szMsg1,
                alias);
#else
        sprintf(msg,
                "%s %s!",
                "Can not find implementation definition for",
                alias);
#endif

        cleanup(ev, shouldRegister);

        WinMessageBox(HWND_DESKTOP,
                      HWND_DESKTOP,
                      msg,
                      "Error!",
                      0L,
                      MB_OK | MB_ERROR);

        DosExit(EXIT_PROCESS, 1);
       }

       if (shouldRegister)
       {
         DosExitList(EXLST_ADD, (PFNEXITLIST) serverExitHandlerRegistered);
       }
       else
       {
         //Display message box that avsshell is starting
         DosExitList(EXLST_ADD, (PFNEXITLIST) serverExitHandlerUnregistered);
       }

    // Implementation now ready to process requests
    SOMD_SOMOAObject = new SOMOA;
    SOMD_SOMOAObject->impl_is_ready(ev, SOMD_ImplDefObject);

    if (ev->_major != NO_EXCEPTION)
    {
#ifdef ODDebug
      somPrintf("impl_is_ready failed, exception = %s\n", somExceptionId(ev));
#endif
      cleanup(ev, shouldRegister);
      DosExit(EXIT_PROCESS, 1);
    }

      rc = DosCreateThread(&fDSOMThreadID,
                           (PFNTHREAD)dsomThread,
                           (ULONG)shouldRegister,
                           0,
                           0x10000);

       if (rc != 0)
       {
         cleanup(ev, shouldRegister);
         WinMessageBox(HWND_DESKTOP,
                       HWND_DESKTOP,
                       "Cannot start DSOM thread!",
                       "Error!",
                       0L,
                       MB_OK | MB_ERROR);
         DosExit(EXIT_PROCESS, 1);
       }
    }
    else
    {
       SOM_InitEnvironment(ev);
    }
}

SOM_Scope ODPlatformWindow  SOMLINK ODSessionCreatePlatformWindow(ODSession *somSelf,
                                                                   Environment *ev,
                                                                   ODPlatformWindowCreateOptions options)
{
    ODSessionData *somThis = ODSessionGetData(somSelf);
    ODSessionMethodDebug("ODSession","ODSessionCreatePlatformWindow");

    /* Return statement to be customized: */
    { ODPlatformWindow retVal;  return (retVal); }
}

SOM_Scope ODPlatformWindow  SOMLINK ODSessionCreatePartWindow(ODSession *somSelf,
                                                                   Environment *ev,
                                                                   ODPlatformWindowCreateOptions options,
                                                                   SWP swp)
{
    ODSessionData *somThis = ODSessionGetData(somSelf);
    ODSessionMethodDebug("ODSession","ODSessionCreatePartWindow");

    /* Return statement to be customized: */
    { ODPlatformWindow retVal;  return (retVal); }
}



//------------------------------------------------------------------------------
// ODSession: GetLinkService
//------------------------------------------------------------------------------

SOM_Scope IODLinkService *   SOMLINK ODSessionGetLinkService(ODSession *somSelf, Environment *ev)
{
    ODSessionData *somThis = ODSessionGetData(somSelf);
    ODSessionMethodDebug("ODSession","GetLinkService");
    return(_fLinkService);
}

// *=============================================================================
// * ODSession: StartServer
// *=============================================================================

SOM_Scope void
SOMLINK ODSessionStartServer(ODSession *somSelf,
                             Environment *ev,
                             string alias)
{
    ODSessionData *somThis = ODSessionGetData(somSelf);
    ODSessionMethodDebug("ODSession","ODSessionStartAvailServer");

    // Start DSOM, using the specified alias, and assuming preregistered

  try {
    static ODBoolean serverStarted = kODFalse;
    if (!serverStarted)
    {
      ULONG shouldRegister = (alias == kODNULL) ? 1 : 0;
      startup(ev, (char*)alias, shouldRegister, somThis);
      serverStarted = kODTrue;
    }

   }
   catch (ODException _exception) {
         SetErrorCode(kODNoError);
   }
}

#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)

// returns a pointer to the help instance.
SOM_Scope ODHelp*  SOMLINK ODSessionGetHelp(ODSession *somSelf, Environment *ev)
{
  ODSessionData *somThis = ODSessionGetData(somSelf);
  ODSessionMethodDebug("ODSession","ODSessionGetHelp");
  return _fHelp;
}

#endif  //_PLATFORM_WIN32_ || _PLATFORM_OS2_

/* method: SetLinkONOFFflag */
SOM_Scope void  SOMLINK ODSessionSetLinkONOFFflag(ODSession *somSelf, Environment *ev,
                                                        char flag)
{

    ODSessionData *somThis = ODSessionGetData(somSelf);
    ODSessionMethodDebug("ODSession","ODSessionSetLinkONOFFflag");

    _fLinkONOFFflag = flag;
}



/* method: GetLinkONOFFflag */
SOM_Scope char SOMLINK ODSessionGetLinkONOFFflag(ODSession *somSelf, Environment *ev)
{

    ODSessionData *somThis = ODSessionGetData(somSelf);
    ODSessionMethodDebug("ODSession","ODSessionSetLinkONOFFflag");


    return( _fLinkONOFFflag );

}
