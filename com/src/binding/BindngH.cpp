/* @(#)Z 1.14 com/src/binding/BindngH.cpp, odconfig, od96os2, odos29646d 96/11/15 15:24:42 (96/10/29 09:15:00) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odconfig
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

/*
  File:    BindngH.cpp

  Contains:  C++ Implementation for Binding Helper class.

  Owned by:  Caia Grisar

  Copyright:  © 1993-94 by Apple Computer, Inc., all rights reserved.

  Change IBM History (most recent first):

     <14> 04/29/96 LAW          further changes for NT version
     <13> 04/11/96 CBH          Ported to NT platform
     <12> 11/27/95 JSO 143039   Added check for editorexists in alphanumchooseeditor
     <11> 11/01/95 JSO 142527   Added support track registry updates
     <10> 09/29/95 JSO 123852   Added support for preferred categories
     <9>  08/23/95 JSO 133968   port dr3
     <8>  07/27/95 JSO 124094   Fix to prevent trap if dll is missing
     <7>  07/21/95 JSO 130450   Removed old filetypepreferences code
     <6>  07/14/95 JSO 123473   Added Pref FileType/Preferences calls
     <5>  06/28/95 JSO 123479   Commented out Update Preferences call
     <4>  06/09/95 JSO 125065   Updates for System Preferences
     <3>  06/05/95 jso 123330     Fix for Kind/Category matching
     <2>  04/28/95 jso 120604  B1C16 Port Apple Changes (24 - 28>
     <1>  04/13/95 dcp # 117196 : Added null check for containerType in GetContainerSuite method.

  Change Apple History (most recent first):

    <37>   6/21/95  CC    1260937: BB: ODTypeListIterator must return
                  copies of ODType values.
    <36>   6/19/95  VL    1170098: Set return result for
                  GetPreferredEditorForPart to kODNULL if the
                  operation fails.
    <35>   6/16/95  EL    #1254926: Don't return an editor that does
                  not exists.
    <34>   6/16/95  CC    1259719: Added ChangeEditor and
                  ChangeKindAndEditor.
    <33>   5/18/95  CG    #1249728 BB: BindingH.cpp needs to call
                  StdTypIO routines for ISOStr
                  #1249263 BB: BndNSUtl.cpp does not compile
                  standalone.
    <32>   4/27/95  CC    1243813: Removed ShowAlert and
                  BindingDialogFilter; replaced by ShowAlert
                  in DlogUtil.
    <31>   4/26/95  CG    1210975 BB: 5$ Bugs need to be evaluatedand
                  removed from Binding.
    <30>   4/26/95  CG    1210975 BB: 5$ Bugs need to be evaluated
                  and removed from Binding.
    <29>   4/13/95  CG    Removed obsolete #include.
    <28>   3/28/95      CG      #1211901 BB:  Binding methods check for
                                null storage unit.
    <27>   2/16/95      CG      #1213926 BB:binding respects preferred
                                kinds.  #1210975 BB: $$$$$ fixes.
    <26>   2/14/95      TJ      Fixed copyright date.
    <25>   2/14/95      jpa     Fixed GetContainerSuite to always return
                                NULL on failure, and to use the
                                SysPrefContainerSuites namespace. [1205141]
    <24>    2/2/95      CG      #1195019:  ValueNameSpace entries are now
                                copied in and out instead of pointers being
                                passed around.
    <23>   1/25/95  jpa    Fixed no-prefs crashes [1214750]
    <22>   1/24/95  CG    In GetContainerSuite now check return value
                  of ValueNameSpaceGetEntry instead of return
                  parameter.  broke last build.
    <21>   1/23/95  CG    #1195019: ValueNameSpace methods now take
                  ODByteArray parameter instead of ODPtr.
    <20>   1/13/95  RR    Removed include of ODTypesM.xh
    <19>   1/12/95  jpa    Don't use obsolete Toolbox names. [1211211]
    <18>   1/10/95  CG    #1201731:  BB: Now get NoPart if a part
                  can't be loaded because of an error.
    <17>  12/20/94  VL    1195012: Make Storage calls be
                  marshallable.
    <16>  12/13/94  CG    #1193522: Warning dialog displayed if
                  automatic binding has occurred.
    <15>   10/7/94  CG    #1184728:  Broke NoPart with last fix!
    <14>   10/7/94  CG    #1184728: Binding to another part that
                  supports the same kind when original editor
                  not found now works.
    <13>    9/9/94  CG    1182933: changed NoPart editor id.
    <12>    9/9/94  jpa    Fixed IsEditorLoaded to call new
                  ODClassExists rather than use SOM; this fix
                  re-enables debugging of parts. [1182105]
    <11>   8/24/94  CG    #1181622: Updated file ownership.
    <10>   8/17/94  CG    #1181440 The View and EditorKinds name
                  spaces were changed to be of type
                  ODObjectNameSpace.
     <9>   7/29/94  CG    NameSpace API changes.
     <8>   7/26/94  jpa    Don't call InitObject from InitBinding.
     <7>    7/1/94  CG    Changes to IsEditorLoaded.
     <6>    7/1/94  CG    Added som calls to IsEditorLoaded.
     <5>   6/28/94  VL    Use Translt.cpp.
     <4>   6/27/94  CG    Moved IsEditorLoaded here from Prefs.
                  Removed fEv.
     <3>   6/23/94  eeh    oops....include EditrSet.h
     <2>   6/23/94  eeh    remove ODEditorSetIterator and EditorSet
     <1>   6/20/94  CG    first checked in

  To Do:
  In Progress:
*/

#ifndef _BINDNGH_
#include "BindngH.h"
#endif

#ifdef  _PLATFORM_MACINTOSH_
#ifndef __ERRORS__
#include <Errors.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_OS2_ // For the updating preferences on OS2
#include <sys\types.h>
#define INCL_DOS
#include <OS2.H>
#endif // _PLATFORM_OS2_

#ifndef _EDITRSET_
#include "EditrSet.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef SOM_ODStorageUnit_xh
#include "StorageU.xh"
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include "StdDefs.xh"
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include "StdProps.xh"
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include "StdTypes.xh"
#endif

#ifndef _ODUTILS_
#include "ODUtils.h"
#endif

#ifndef SOM_ODTranslation_xh
#include "Translt.xh"
#endif

#ifndef _ODSESSN_
#include "ODSessn.xh"
#endif

#ifndef SOM_ODValueNameSpace_xh
#include "ValueNS.xh"
#endif

#ifndef SOM_ODObjectNameSpace_xh
#include "ObjectNS.xh"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include "NmSpcMg.xh"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef SOM_Apple_NoPart_xh
//#include <NoPart.xh>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif

#ifndef _ODNEWOBJ_
#include <ODNewObj.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __DIALOGS__
#include <Dialogs.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef SOM_ODWindowState_xh
#include <WinStat.xh>
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _USERSRCM_
#include <UseRsrcM.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _BNDNGDEF_
#include "BndngDef.h"
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifndef _NMSPCUTL_
#include <NmSpcUtl.h>
#endif

#ifndef SOM_ODDispatcher_xh
#include <Disptch.xh>
#endif

#ifndef _DLOGUTIL_
#include <DlogUtil.h>
#endif

#ifndef SOM_ODPartWrapper_xh
#include <PartWrap.xh>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#ifndef SOM_ODObjectIterator_xh
#include <ObjctItr.xh>
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef _RManager_
#include <RManager.xh>
#endif

#ifndef _ODPrtRei_
#include <ODPrtRei.xh>
#endif

#ifndef _PartInfi_
#include <PartInfi.xh>
#endif

#include <somcls.xh>
#include <somcm.xh>
#include <sys/stat.h>
#include <iostream.h>
#if defined(_PLATFORM_WIN32_) || defined(_PLATFORM_OS2_)
#include <direct.h>
#endif

#undef  LOGGING   // remove define of logging (off) from ODDebug.h
#define LOGGING 1 // turn on logging

//==============================================================================
// Constants
//==============================================================================
const int MAXSTR = 256;

//=========================================================================
// Globals
//=========================================================================


//=========================================================================
// Function Prototypes
//=========================================================================
#ifdef _PLATFORM_MACINTOSH_

pascal Boolean BindingDialogFilter(DialogPtr dialog,
                                   EventRecord* event,
                                   short* itemHit);
#endif // _PLATFORM_MACINTOSH_
//==============================================================================
// Class Binding
//==============================================================================

//------------------------------------------------------------------------------
// Binding::Binding
//------------------------------------------------------------------------------
Binding::Binding()
{
  fSession    = (ODSession*) kODNULL;
  fRManager   = 0;
  fODRegistry = 0;
  lastRegUpdate = 0;
  fev = somGetGlobalEnvironment();
}

//------------------------------------------------------------------------------
// Binding::~Binding
//------------------------------------------------------------------------------
Binding::~Binding()
{
}

//------------------------------------------------------------------------------
// Binding::InitBinding
//------------------------------------------------------------------------------
void Binding::InitBinding(ODSession* session )
{
  char *dirname;
  struct stat statbuf;

  fSession = session;
  fRManager = new RegistryManager;
  fODRegistry = (ODPartHandlerRegistryInt*)fRManager->
                        GetODPartHandlerRegistry(somGetGlobalEnvironment());

  // 24247: set global registry reference in session object
  Environment* ev = somGetGlobalEnvironment ();
  session->SetRegistry(ev, fODRegistry);

  dirname = getenv(ODRegistryPathEnvVar);
  if ((dirname == NULL) || (*dirname == 0))
     {
     getcwd((char *) &completeRegFileName, MAXSTR);
     strcat(completeRegFileName, "\\");
     strcat(completeRegFileName, ODRegistryFileName);
     }
  else
     {
     strcpy(completeRegFileName, dirname);
     strcat(completeRegFileName, "\\");
     strcat(completeRegFileName, ODRegistryFileName);
     }
//   LOG("OpenDoc Registry file = %s\n", completeRegFileName);

#ifdef _PLATFORM_UNIX_
  if(stat(this->completeRegFileName, &statbuf) == 0)
#else
  if(_stat(this->completeRegFileName, &statbuf) == 0)
#endif
     this->lastRegUpdate = statbuf.st_atime;
  else
     this->lastRegUpdate = 0;
}

//------------------------------------------------------------------------------
// Binding::Purge
//------------------------------------------------------------------------------
ODSize Binding::Purge(ODSize size)
{
ODUnused(size);
  // The only memory that Binding has to purge is owned by NmSpcMgr and
  // should be purged from there.
  return 0;
}


  //---------------------------------
  // Direct editor bindings

//------------------------------------------------------------------------------
// Binding::ChooseEditorForPart
//------------------------------------------------------------------------------
ODEditor Binding::ChooseEditorForPart(ODStorageUnit* thePartSU, ODType newKind )
{
  struct stat statbuf;
  ODEditor theEditor = (ODEditor)kODNULL;
  ODBoolean noPreferredEditorProp = kODFalse;

  if (thePartSU)
     theEditor = GetPreferredEditorForPart(thePartSU, &noPreferredEditorProp);
  if (theEditor) return theEditor;

 // check the last update of the registry file
  long update = 0;
#ifdef _PLATFORM_UNIX_
  if(stat(this->completeRegFileName, &statbuf) == 0)
#else
  if(_stat(this->completeRegFileName, &statbuf) == 0)
#endif
  {
     update = statbuf.st_atime;
     if(update > this->lastRegUpdate)
     {
       fODRegistry = (ODPartHandlerRegistryInt*) fRManager->RefreshODPartHandlerRegistry (fev);
       fSession->SetRegistry(fev, fODRegistry);
       this->lastRegUpdate = update;
       ODNameSpaceManager *nsm = fSession->GetNameSpaceManager(fev);
       nsm->SynchNSTable(fev);
     }
  }

  if (thePartSU)
     theEditor = ChooseDirectEditorForPart(thePartSU, newKind);

/*
  // NO AUTOMATIC TRANSLATION in v1
  if (!theEditor && AutomaticTranslation())
    theEditor = TranslateAndChooseEditorForPart(thePartSU);
*/

  if( !theEditor)
      theEditor = ChooseEditorForOSType(thePartSU);

  if( !theEditor)
      theEditor = ChooseEditorForOSfilenameFilter(thePartSU);

  if (!theEditor)
  {
    theEditor = (ODEditor) kODBlackBoxHandlerOfLastResort;
  }
  else
  {
        if (thePartSU)
        {
                ODDraft* draft = thePartSU->GetDraft(fev);
                if (draft->GetPermissions(fev) == kODDPExclusiveWrite)
                {
                        ODSetISOStrProp(fev, thePartSU,
                                        kODPropPreferredEditor,
                                        kODEditor,
                                        theEditor);
                }
                if (!noPreferredEditorProp) this->BindingWarning();
        }
  }
  return theEditor;
}

//------------------------------------------------------------------------------
// Binding::TranslateAndChooseEditorForPart
//------------------------------------------------------------------------------

ODEditor Binding::TranslateAndChooseEditorForPart(ODStorageUnit* thePartSU)
{
ODUnused(thePartSU);
  return (ODEditor)kODNULL;
}

//------------------------------------------------------------------------------
// Binding::ChooseDirectEditorForPart
//------------------------------------------------------------------------------

ODEditor Binding::ChooseDirectEditorForPart(ODStorageUnit* thePartSU,
                                ODType newKind )
{
// NOTE: This method does NOT check the preferred editor of the given part

  ODEditor theEditor = kODNULL;

   if (thePartSU)
   {
      // Check for preferredKind property
      if (thePartSU->Exists(fev, kODPropPreferredKind, kODISOStr, 0))
      {
        ODULong size;
        ODType preferredKind = ODGetISOStrProp(fev, thePartSU,
                  kODPropPreferredKind, kODISOStr, kODNULL, &size);
        theEditor = ChooseDirectEditorForKind(preferredKind,thePartSU);
        ODDisposePtr(preferredKind);
      }
      if (theEditor) return theEditor;

      // Attempt to bind using the content values in their fidelity order
      for (ODValueIndex valueIndex = 1;
        thePartSU->Exists(fev, kODPropContents,(ODValueType)kODNULL,valueIndex)
                                && (theEditor == kODNULL);
        valueIndex++)
      {
        thePartSU->Focus(fev, kODPropContents,kODPosSame,(ODValueType)kODNULL,
                                 valueIndex,kODPosSame);
        theEditor = ChooseDirectEditorForKind(thePartSU->GetType(fev),thePartSU);
      }
      if (theEditor) return theEditor;
   }

   theEditor = ChooseDirectEditorForKind(newKind,thePartSU);

   if (theEditor) return theEditor;
   else return kODNULL;
}

//------------------------------------------------------------------------------
// Binding::ChooseDirectEditorForKind
//------------------------------------------------------------------------------

ODEditor Binding::ChooseDirectEditorForKind(ODType aKind,
                             ODStorageUnit* thePartSU)
{

  ISOString partHandler = NULL;
  ODPartHandlerInfo* partHandlerInfo = kODNULL;
  ODEditor theEditor = kODNULL;
  ODSShort               i = 0;
  _IDL_SEQUENCE_string CategoryList;
  _IDL_SEQUENCE_ISOString PartHandlerList;
  ODBoolean   editorFound = kODFalse;
  Environment* ev = somGetGlobalEnvironment ();

  if (!aKind )
    return theEditor;


   // Check for the preferred editor first
   partHandler = fODRegistry->GetPreferredPartHandler(fev, aKind);
   if(partHandler)
   {
      partHandlerInfo = fODRegistry->GetPartHandlerInfo( fev, partHandler);
      theEditor = (ISOString) ODNewPtr(MAXSTR);
      string className = partHandlerInfo->GetPartHandlerClassName(fev);
      strcpy(theEditor, className);

      SOMFree(partHandler);
      SOMFree(partHandlerInfo);
      SOMFree(className);

//      if (theEditor && EditorExists(theEditor))
      if (theEditor)
        return theEditor;
      else
      {
        ODDisposePtr( theEditor );
        theEditor = kODNULL;
      }
   }

  // retrieve Category list from Registry
  // this is a buffer that the Registry allocates and we need to dispose of.
  CategoryList = fODRegistry-> GetCategoryList( ev, 0, aKind);

  if (CategoryList. _length )
  {
     // search the Category list for  Editiors with matching kind
     for ( i = 0; i < (CategoryList. _length); i++)
     {
         // Check for the preferred editor first
         partHandler = fODRegistry-> GetPreferredPartHandlerForCategory(fev,
                                        CategoryList. _buffer[i]);
         if(partHandler)
         {
            partHandlerInfo = fODRegistry->GetPartHandlerInfo( fev, partHandler);
            theEditor = (ISOString) ODNewPtr(MAXSTR);
            string className = partHandlerInfo->GetPartHandlerClassName(fev);
            strcpy(theEditor, className);

            SOMFree(partHandler);
            SOMFree(partHandlerInfo);
            SOMFree(className);

            editorFound = (theEditor &&
                 EditorSupportsKind_Reg(theEditor,  aKind, fSession ) );
         }

     }

     // now free up the buffer we got from the Registry
     for ( i = 0; i < CategoryList. _length; i++)
     {
         SOMFree( CategoryList. _buffer[i]);
     }
     SOMFree( CategoryList. _buffer);

     if (editorFound) return theEditor;
     else
     {
       ODDisposePtr( theEditor ); // [120604]
       theEditor = kODNULL;
     }
  }

  EditorSet* editorSet = new EditorSet;
  editorSet->InitEditorSet();

  // retrieve PartHandler list from Registry
  // this is a buffer that the Registry allocates and we need to dispose of.
  // ODSession* session = thePartSU->GetSession(ev);
  PartHandlerList = fODRegistry-> GetPartHandlerList( ev, aKind, 0);
  if ( PartHandlerList. _length )
  {
     // search the Category list for  Editiors with matching kind
     for ( i = 0; i < ( PartHandlerList. _length); i++)
     {
       editorSet->AddEditor(PartHandlerList. _buffer[i]);
     }
    theEditor = AlphaNumChooseEditor(editorSet);
  }
  else
  {
    ODDisposePtr( theEditor );
    theEditor = kODNULL;
  }

  // now free up the buffer we got from the Registry
  for ( i = 0; i < PartHandlerList. _length; i++)
  {
      SOMFree( PartHandlerList. _buffer[i]);
  }
  SOMFree( PartHandlerList. _buffer);


  ODDeleteObject( editorSet );
  return theEditor;
}

  //---------------------------------
  // Editor related utility methods

//------------------------------------------------------------------------------
// Binding::EditorExists
//
//   lookup editor in editorkinds namespace and return kODTrue if it exists
//------------------------------------------------------------------------------

ODBoolean Binding::EditorExists(ODEditor editor)
{

/* defect 22716 - just use Registry
  ODNameSpaceManager *nsm = fSession->GetNameSpaceManager(fev);
*/

  /*  Try twice to find the editor. If the first search fails, re-synch the
        name-space (by hitting the disk and searching all available "Editors"
        folders) and try again. If the second search fails, give up. --jpa */

  /*    ????? This could be slow if we are called many times in a row with
        genuinely nonexistent editors. If this turns out to be a real problem,
        one could keep around the time of last synching, and not synch more than
        once every five seconds. */

/* defect 22716 - just use Registry

  ODBoolean synched = kODFalse;
  for( ;; ) {
        ODNameSpace* editorkindsNameSpace = fSession->
                GetNameSpaceManager(fev)->HasNameSpace( fev, kODEditorKinds );

        if ( editorkindsNameSpace &&
             editorkindsNameSpace->Exists(fev, (ODISOStr)editor) &&
             this->IsEditorLoaded(editor) )
                return kODTrue;

        if( !synched ) {
                synched = kODTrue;
                nsm->SynchNSTable(fev);
                continue;
        }
        else return kODFalse;
  }
*/
  ODPartHandlerInfo* pinfo = kODNULL;
  pinfo = fODRegistry->GetPartHandlerInfo(fev, (ODISOStr)editor);
  if (pinfo)  // the editor is registered
        return kODTrue;
  else
        return kODFalse;

}

//------------------------------------------------------------------------------
// Binding::IsEditorLoaded
//------------------------------------------------------------------------------
ODBoolean Binding::IsEditorLoaded(ODEditor editor)
{
  return ODClassExists(editor);
}

//------------------------------------------------------------------------------
// Binding::GetPreferredEditorForPart
//------------------------------------------------------------------------------
ODEditor Binding::GetPreferredEditorForPart(ODStorageUnit* thePartSU,
                        ODBoolean* noPreferredEditorProp)
{
  // read preferred editor from thePartSU
  ODEditor preferredEditor = kODNULL;

  // does the PreferredEditor property exist for this SU?
  if (thePartSU->Exists(fev, kODPropPreferredEditor,kODEditor,0))
  {
    ODULong size;
    preferredEditor = ODGetISOStrProp(fev, thePartSU,
                                        kODPropPreferredEditor,
                                        kODEditor, kODNULL, &size);
    if (preferredEditor)
    {
        if (EditorExists(preferredEditor))
                *noPreferredEditorProp = kODFalse;
        else
        {
                ODDisposePtr(preferredEditor);
                preferredEditor = kODNULL;
        }
    }
  }
  else // no PreferredEditor property for this SU, make one
  {
    ODDraft* draft = thePartSU->GetDraft(fev);

    if (draft->GetPermissions(fev) == kODDPExclusiveWrite)
        thePartSU->AddProperty(fev, kODPropPreferredEditor)->
                        AddValue(fev, kODEditor);

    *noPreferredEditorProp = kODTrue;
  }
  return preferredEditor;
}


//------------------------------------------------------------------------------
// Binding::GetKindsSupported
//
//   lookup editor in editorkinds namespace and return kinds supported
//------------------------------------------------------------------------------

ODTypeList* Binding::GetKindsSupported(ODEditor editor)
{
  ODTypeList*  kindsList = kODNULL;

  // get the editorkinds namespace
  ODObjectNameSpace* editorkindsNameSpace = (ODObjectNameSpace*)fSession->
                GetNameSpaceManager(fev)->HasNameSpace( fev, kODEditorKinds );

  if (editorkindsNameSpace)
  {
    // Lookup the kinds list by editor class ID
    if (editorkindsNameSpace->
        GetEntry( fev, (ODISOStr)editor, (ODObject**)&kindsList ))
                return kindsList;
  }

  return kindsList;
}

//------------------------------------------------------------------------------
// Binding::AlphaNumChooseEditor
//------------------------------------------------------------------------------

ODEditor Binding::AlphaNumChooseEditor(EditorSet* editorSet)
{
  ODEditor bestEditor = (ODEditor)kODBlackBoxHandlerOfLastResort;

  EditorSetIterator* eit = editorSet->CreateIterator();

  for (ODEditor editor = eit->First();
    eit->IsNotComplete();
    editor = eit->Next())
  {
    if(!EditorExists(editor))
       continue;
    bestEditor = AlphaNumChooseAnEditorFromTwo(editor,bestEditor);
  }

  ODDeleteObject( eit );
  return bestEditor;
}

//------------------------------------------------------------------------------
// Binding::AlphaNumChooseAnEditorFromTwo
//------------------------------------------------------------------------------

ODEditor Binding::AlphaNumChooseAnEditorFromTwo(ODEditor e1, ODEditor e2)
{
  ODObjectNameSpace*  theViewerNameSpace = ViewerNameSpace();
  ODObject*  ignore;
  ODBoolean e1IsViewer = kODFalse;
  ODBoolean e2IsViewer = kODFalse;

  if (theViewerNameSpace)
  {
    // determine if e1 or e2 are viewers (as opposed to editors)
    e1IsViewer = theViewerNameSpace->GetEntry(fev, (ODISOStr)e1, &ignore);
    e2IsViewer = theViewerNameSpace->GetEntry(fev, (ODISOStr)e2, &ignore);
  }

  // if e1 is not a viewer and e2 is, use e1
  if (!e1IsViewer && e2IsViewer) return e1;

  // if e2 is not a viewer and e1 is, use e2
  if (!e2IsViewer && e1IsViewer) return e2;

  // which editor name comes first, alphanumerically?
  if ( strcmp( (char*)e1, (char*)e2 ) >= 0)
    return e1;
  else
    return e2;
}

//------------------------------------------------------------------------------
// Binding::DraftRestrictsEditors
//------------------------------------------------------------------------------

ODBoolean Binding::DraftRestrictsEditors(ODDraft* draft)
{
ODUnused(draft);
  // return whether or not restrictedEditors property exists
  return kODFalse;
}

//------------------------------------------------------------------------------
// Binding::GetAllRestrictedEditorsForKind
//------------------------------------------------------------------------------

EditorSet* Binding::GetAllRestrictedEditorsForKind(ODDraft* draft, ODType kind)
{
ODUnused(draft);
ODUnused(kind);
  // read and return restrictedEditors property of draftProperties
  return kODNULL;
}

  //---------------------------------
  // Category and kind mappings

//------------------------------------------------------------------------------
// Binding::ViewerNameSpace
//------------------------------------------------------------------------------
ODObjectNameSpace*  Binding::ViewerNameSpace()
{
  return (ODObjectNameSpace*)fSession->
                GetNameSpaceManager(fev)->HasNameSpace( fev, kODViewer );
}

ODContainerSuite Binding::GetContainerSuite(ODContainerType containerType)
{


  /* Two things can happen here. If containerType is the name of an actual
    container suite, we look it up in the ContainerSuite namespace and
    return the classname. But containerType might also be a constant
    indicating a type of default container suite (e.g. kODDefaultFileContainer)
    in which case we have to first look it up in the system-preferred
    ContainerSuite namespace to get the real container suite name, then
    do the lookup as above to get the classname. */

  ODContainerSuite   theContainerSuite;

  // First look it up in the ContainerSuite namespace:
  theContainerSuite = ValueNameSpaceGetString(fSession,
                                                kODContainerSuite,
                                                containerType);
  if( !theContainerSuite ) {
    ODContainerSuite realType;
    ODBoolean isDefault = kODFalse;
    // Not found, look it up in the System-preferred Container Suites namespace:
    realType = ValueNameSpaceGetString(fSession,kODSysPrefContainerSuites,containerType);
    if( !realType ) {
      // Not found there either, check some hardcoded defaults:
      isDefault = kODTrue;
      if( ODISOStrEqual(containerType,kODDefaultMemoryContainer) )
        realType = kODBentoMemoryContainer;
      else if( ODISOStrEqual(containerType,kODDefaultFileContainer) )
        realType = kODBentoFileContainer;
    }
    if( realType ) {
      // Now that we have a real containerType, look it up in ContainerSuite:
      theContainerSuite = ValueNameSpaceGetString(fSession,kODContainerSuite,realType);
      if( !isDefault )
        ODDisposePtr(realType);
    }
  }
  return theContainerSuite;
}


//------------------------------------------------------------------------------
// Binding::BindingWarning
//------------------------------------------------------------------------------

void Binding::BindingWarning()
{

#ifdef _PLATFORM_MACINTOSH_
  // [120604] begin changes
  Environment* ev = somGetGlobalEnvironment ();

  fSession->GetWindowState(ev)->DeactivateFrontWindows(ev);
  ShowAlert(ev, kBindingWarning, GetODButtonKeyFilterProc(), fSession);
  fSession->GetWindowState(ev)->ActivateFrontWindows(ev);

  ResetAlertStage();
#endif
}

//------------------------------------------------------------------------------
// IsNoPart
//------------------------------------------------------------------------------

ODBoolean EXPENTRY  IsNoPart(Environment* ev, ODPart* rootPart)
{
  // ask the PartWrapper what editor it is using
  ODEditor editor = ((ODPartWrapper*)rootPart)->GetEditor(ev);

  // get the length of the editor name
  ODUShort length = (ODUShort)ODISOStrLength(editor);

  // return true if name == "NoPart", false otherwise
  return
     (ODISOStrNCompare(editor, kODBlackBoxHandlerOfLastResort, length) == 0);
}

//------------------------------------------------------------------------------
// GetCurrentEditorForPart
//------------------------------------------------------------------------------
ODEditor EXPENTRY  GetCurrentEditorForPart(ODPart* part )
{
  Environment* fev = somGetGlobalEnvironment();

  // call the PartWrapper and ask what editor it is using
  ODEditor theEditor = ((ODPartWrapper*)part)->GetEditor(fev);

  // allocate a buffer to hold the editor name and copy the name
  ODUShort length = (ODUShort)ODISOStrLength(theEditor);
  ODEditor partName = (ODEditor)ODNewPtrClear( length+1 );;
  ODBlockMove(theEditor,  partName, length+1 );

  return partName;
}

//------------------------------------------------------------------------------
// Binding::ChooseEditorForOSType
//------------------------------------------------------------------------------

ODEditor Binding::ChooseEditorForOSType(ODStorageUnit* thePartSU )
{

// This method chooses an editor for a dropped-in filetype based on
// the fileTypeEA, if it has one which matches a part's filetypeFilter

   ODEditor theEditor = kODNULL;
   ODULong size = 0;
   char  fileType[MAXSTR];

// Extended Attributes not supported on any platform except OS/2
#ifdef _PLATFORM_OS2_

   if(thePartSU->Exists(fev, kODPropContents,kODFileTypeEA,0) )
   {
       thePartSU->Focus(fev, kODPropContents, kODPosUndefined,
                           (ODValueType)kODFileTypeEA, 0, kODPosUndefined);
       size = thePartSU->GetSize(fev);
       if(!size)
          return theEditor;
       StorageUnitGetValue(thePartSU, fev, size, fileType);

       if(!strcmp("OpenDoc Document", fileType) )
          return theEditor;

       // Check for the preferred editor first
       ISOString partHandler = NULL;
       ODPartHandlerInfo* partHandlerInfo;
       partHandler = fODRegistry-> GetPreferredPartHandlerForFileType(fev,
                                      fileType);
       if(partHandler)
       {
          partHandlerInfo = fODRegistry->GetPartHandlerInfo( fev, partHandler);
          theEditor = (ISOString) ODNewPtr(256);
          string className = partHandlerInfo->GetPartHandlerClassName(fev);
          strcpy(theEditor, className);

          SOMFree(partHandler);
          SOMFree(partHandlerInfo);
          SOMFree(className);

          return theEditor;

       }
    }
#endif  // _PLATFORM_OS2_

    return theEditor;
}
//------------------------------------------------------------------------------
// Binding::ChooseEditorForOSfilenameFilter
//------------------------------------------------------------------------------

ODEditor Binding::ChooseEditorForOSfilenameFilter(ODStorageUnit* thePartSU )
{

// This method chooses an editor for a dropped-in filetype based
// on the fileExtension, if it has one which matches a part's filetypeFilter

   ODEditor theEditor = kODNULL;
   ODULong size = 0;
   char  fileName[MAXSTR];
   int found = kODFalse;

    if(thePartSU->Exists(fev, kODPropContents,kODFileType,0) )
    {
        thePartSU->Focus(fev, kODPropContents, kODPosUndefined,
                                  kODFileType,  0, kODPosUndefined);
        size = thePartSU->GetSize(fev);
        if(!size || size > MAXSTR) return theEditor;

        StorageUnitGetValue(thePartSU, fev, size, fileName);
        char * b = fileName;
        char * searchPointer = strchr(fileName, '\0');
        while(searchPointer >= b)
        {
                if(*searchPointer == '.')
                {
                   found = kODTrue;
                   break;
                }
                searchPointer--;
        }

        if(!found) return theEditor;

       char *  searchString = (char *)ODNewPtrClear(strlen(searchPointer)+1, 0);
       strcpy(searchString, "*");
       strcat(searchString, searchPointer);

       // Check for the preferred editor first
       ISOString partHandler = NULL;
       ODPartHandlerInfo* partHandlerInfo;
       partHandler = fODRegistry-> GetPreferredPartHandlerForFileExt(fev,
                                      searchString);
       if(partHandler)
       {
          partHandlerInfo = fODRegistry->GetPartHandlerInfo( fev, partHandler);
          theEditor = (ISOString) ODNewPtr(MAXSTR);
          string className = partHandlerInfo->GetPartHandlerClassName(fev);
          strcpy(theEditor, className);

          SOMFree(partHandler);
          SOMFree(partHandlerInfo);
          SOMFree(className);

          return theEditor;
       }
    }

    return theEditor;
}
