/* @(#)Z 1.7 com/src/binding/BindngH.h, odconfig, od96os2, odos29646d 96/11/15 15:29:15 (96/10/29 09:15:07) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odconfig
 *
 *   CLASSES:   Binding
 *
 *   ORIGINS: 82,27
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
/*  IBM Change History (most recent first):                         */
/*  142527 11/01/95 jso Added fields to track partreg.bto           */
/*  140177 10/13/95 ced make extern C function EXPENTRY             */
/*  <OS2>  08/22/95 aml 133968 Port DR3                             */
/*                                                                  */
/********************************************************************/
/*
  File:    BindngH.h

  Contains:  Class definition for Binding helper object

  Owned by:  Caia Grisar

  Copyright:  © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

  Change History (most recent first):

     <4>   6/16/95  CC    1259719: Added ChangeEditor and
                  ChangeKindAndEditor.
     <3>   5/18/95  CG    #1249263 BB: BndNSUtl.cpp does not compile
                  standalone.
     <2>   4/27/95  CC    1243813: Removed ShowAlert and
                  BindingDialogFilter; replaced by ShowAlert
                  in DlogUtil.
     <1>   4/13/95  CG    first checked in
     <8>  12/13/94  CG    #1193522: Added Binding Alert.
     <7>   8/24/94  CG    #1181622: Updated ownership info.
     <6>   8/17/94  CG    #1181440: ViewNameSpace now return
                  ODObjectNameSpace.  Made methods
                  nonvirtual.
     <5>   7/29/94  CG    NameSpace API changes.
     <4>   7/26/94  jpa    Don't inherit from ODObject.
     <3>   6/27/94  CG    Moved IsEditorLoaded here from Prefs,
                  Removed fEv
     <2>   6/23/94  eeh    remove EditorSet and ODEditorSetIterator
     <1>   6/15/94  CG    first checked in
     <1>    6/1/94  CG    first checked in
    <14>   5/27/94  CG    #1165186: Abstracted out preferences from
                  Binding object.
    <13>   4/13/94  T‚    #1147222: Fix the View As popup in the Part
                  Frame Info dialog.  Also fix the Category
                  text, and the Kind & Editor popups to show
                  the current kind & editor respectively.
                  Made one method public for PtFrInfo.cpp to
                  use.
    <12>    4/4/94  CC    GetKindsSupported() and
                  GetCategoriesOfKind() return ODTypeList
                  (1153046).
    <11>   3/26/94  NP    1152831. Move private functions here.
    <10>   3/23/94  CG    Added VerifyPrefFileVersion() and
                  UpdatePrefFileVersion() private methods.
     <9>   3/15/94  MB    Changes to support SCpp/ASLM builds,
                  #1150864.
     <8>   3/14/94  CC    Added GetCurrentEditorForPart() (#1150376);
                  Made EditorSupportsKind() and
                  GetKindsSupported() public for
                  LinkDlgs.cpp.
     <7>   3/11/94  CG    Bug # 1150204 - NMAPs are now loaded
                  whenever mod date of registered library
                  folder changes.
     <6>   2/17/94  eeh    bug #1143340: made various private methods
                  public (to call from EdPrfInf.cpp)
     <5>    2/3/94  CG    Added fHeap field.
     <4>   1/12/94  CG    Removed ODCategorySet.
     <3>  12/16/93  T‚    moved typedef of ODEditor to PlfmType.h
     <2>  12/10/93  CG    Added ASLM class id definition for the
                  ODBinding object.
     <1>   11/7/93  T‚    first checked in
  To Do:
*/

#ifndef _BINDNGH_
#define _BINDNGH_

#ifdef _PLATFORM_AIX_
#define EXPENTRY
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODOBJ_
#include "ODObject.xh"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


//=====================================================================================
// Theory of Operation
//=====================================================================================

/*
  This file describes the ODBinding class.
  ODBinding is a system service that is used to bind part data to part editors.
  ODBinding also includes a number of utility functions, including mapping content
  kinds to categories, providing a list of part editors supporting a given kind, and
  routines to access and set system-wide editor preferences.
*/


//=====================================================================================
// Classes defined in this interface
//=====================================================================================
class Binding;

//=====================================================================================
// Classes used by this interface
//=====================================================================================
class EditorSet;
class ODDraft;
class ODFile;
class ODNameSpaceManager;
class ODObjectNameSpace;
class ODSession;
class ODStorageUnit;
class ODTypeList;
class OrderedCollection;
class OrderedCollectionIterator;
class ODPartHandlerRegistryInt;
class RegistryManager;
class ODPart;

//==============================================================================
// function prototypes
//==============================================================================

#ifdef __cplusplus
extern "C" {
#endif


ODBoolean EXPENTRY  IsNoPart(Environment* ev, ODPart* rootPart);
ODEditor  EXPENTRY  GetCurrentEditorForPart(ODPart* part );
void    EXPENTRY ChangeKindAndEditor (Environment *ev,
                                      ODPart* part,
                                      ODType newKind,
                                      ODEditor newEditor,
                                      ODNameSpaceManager* nsm);


#ifdef __cplusplus
}
#endif

//=====================================================================================
// Class Binding
//=====================================================================================

class Binding
{
public:

  Binding();

    // ODSession creates one instance of Binding
    // which is then shared among clients within the process.

  ODVMethod ~Binding();

  ODNVMethod void InitBinding(ODSession* session);

  ODNVMethod ODSize Purge(ODSize size);

  //---------------------------------
  // Direct editor bindings

    // Returns the "best" available editor for the part
    // may invoke automatic translation
  ODNVMethod ODEditor ChooseEditorForPart
                (ODStorageUnit* thePartSU, ODType newKind );

    // Returns the "best" available editor for a non-opendoc  part
    // i.e. bitmap, metafile, etc
  ODNVMethod ODEditor ChooseEditorForOSType
                (ODStorageUnit* thePartSU );

    // Returns the "best" available editor for a non-opendoc  part
    // based od file extension
  ODNVMethod ODEditor ChooseEditorForOSfilenameFilter
                (ODStorageUnit* thePartSU );

    // attempt to intelligently translate one of the kinds in the part into
    // a kind which an available editor supports, and return that editor
  ODNVMethod ODEditor TranslateAndChooseEditorForPart
                (ODStorageUnit* thePartSU);

  //---------------------------------
  // Editor related utility methods

    // Returns the set of data types supported (without translation) by the
    // argument editor.
  ODNVMethod ODTypeList* GetKindsSupported(ODEditor editor);

  ODNVMethod ODContainerSuite GetContainerSuite(ODContainerType containerType);

    // Returns true if the argument editor is present on this machine.
  ODNVMethod ODBoolean EditorExists(ODEditor editor);

  ODNVMethod ODBoolean  IsEditorLoaded(ODEditor editor);


private:

    // Returns the "best" available editor for the part
    // without translation.
  ODNVMethod ODEditor ChooseDirectEditorForPart
                (ODStorageUnit* thePartSU, ODType newKind );

    // Returns the "best" available editor which supports kind aKind of the part
    // without translation.
  ODNVMethod ODEditor ChooseDirectEditorForKind
                (ODType aKind, ODStorageUnit* thePartSU);

  //---------------------------------
  // Editor related utility methods

    // Returns the editor that last externalized the argument storage unit.
    // Null is returned if that editor is not present on the current machine.
    // noPrefEditor is set to true if no kODPropPreferredEditor property
    // exists in the SU
  ODNVMethod ODEditor GetPreferredEditorForPart
                (ODStorageUnit* thePartSU, ODBoolean* noPrefEditor);

    // Returns the (alphanum preferred editor over viewer) from
    // the set of Editors
  ODNVMethod ODEditor AlphaNumChooseEditor(EditorSet* editorSet);

    // Returns the (alphanum preferred editor over viewer) of the two editors
  ODNVMethod ODEditor AlphaNumChooseAnEditorFromTwo(ODEditor e1, ODEditor e2);

    // Returns true if the argument draft restricts the editors that may be
    // used in this document.
  ODNVMethod ODBoolean DraftRestrictsEditors(ODDraft* draft);

    // returns subset of restricted editors of draft which support kind.
  ODNVMethod EditorSet* GetAllRestrictedEditorsForKind
                (ODDraft* draft, ODType kind);

  //---------------------------------
  // Category and kind mappings

  ODNVMethod ODObjectNameSpace*    ViewerNameSpace();

  //---------------------------------
  // Alert Utilities
  ODNVMethod void BindingWarning();

 // Fields:
  enum  { MAX_FILE_SIZE = 256};
  ODSession*                    fSession;
  RegistryManager*              fRManager;
  ODPartHandlerRegistryInt*     fODRegistry;
  long                          lastPrefUpdate;
  long                          lastRegUpdate;
  char                          completeRegFileName[MAX_FILE_SIZE];
  Environment*                  fev;

};

#endif // _BINDNGH_
