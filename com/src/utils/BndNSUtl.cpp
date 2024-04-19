/* @(#) 1.18 com/src/utils/BndNSUtl.cpp, odconfig, od96os2, odos29646d 10/30/96 18:06:34 [11/15/96 15:28:41] */
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
        File:           BndNSUtl.cp

        Contains:       Binding utilities for EditorSetup.

        Owned by:       Caia Grisar

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.


*/

#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif

#ifndef _EDITRSET_
#include "EditrSet.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef SOM_ODValueNameSpace_xh
#include "ValueNS.xh"
#endif

#ifndef SOM_ODObjectNameSpace_xh
#include "ObjectNS.xh"
#endif

#ifndef _STORAGEU_
#include "StorageU.xh"
#endif

#ifndef _STDDEFS_
#include "StdDefs.xh"
#endif

#ifndef _STDPROPS_
#include "StdProps.xh"
#endif

#ifndef _STDTYPES_
#include "StdTypes.xh"
#endif

#ifndef _PART_
#include "Part.xh"
#endif

#ifndef SOM_ODValueIterator_xh
#include <ValueItr.xh>
#endif

#ifndef SOM_ODObjectIterator_xh
#include <ObjctItr.xh>
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif

#ifndef SOM_ODStorageUnitView_xh
#include <SUView.xh>
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef SOM_ODNameSpaceManager_xh
#include <NmSpcMg.xh>
#endif

#ifndef SOM_ODTypeList_xh
#include <TypeList.xh>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _NMSPCUTL_
#include <NmSpcUtl.h>
#endif

#ifndef _UTILERRS_
#include <UtilErrs.h>
#endif

#ifndef SOM_ODTypeListIterator_xh
#include <TypLsItr.xh>
#endif

#include <somcls.xh>
#include <somcm.xh>

#if defined(_PLATFORM_WIN32_)
#include <mapiwin.h>
#include <wtypes.h>
#include <winbase.h>
#endif // defined(_PLATFORM_WIN32_)


//phase1 - add following includes for additional InfoUtil functions
#ifndef SOM_ODSession_xh
#include <ODSessn.xh>
#endif

#ifndef _STDTYPIO_
#include "StdTypIO.h"
#endif

#ifndef __STRING__
#include <string.h>
#endif

#ifndef SOM_ODTranslation_xh
#include <Translt.xh>
#endif

#ifndef SOM_ODWindowIterator_xh
#include <WinIter.xh>
#endif

#ifndef _UTILDEFS_
#include "UtilDefs.h"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef SOM_ODPart_xh
#include <Part.xh>
#endif

#ifndef SOM_ODBinding_xh
#include <ODBindng.xh>
#endif

#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif

#ifndef _PLFMFILE_
#include <PlfmFile.h>
#endif

#ifndef _ITEXT_
#include <IText.h>
#endif

#ifndef _ODPARTWRAPPER_
#include <PartWrap.xh>
#endif

#ifndef _ODPRTREG_
#include <ODPrtReg.xh>
#endif

#ifndef _ODPRTREI_
#include <ODPrtRei.xh>
#endif

#ifndef SOM_ODFrameFacetIterator_xh
#include <FrFaItr.xh>
#endif

#ifndef SOM_ODFrame_xh
#include <Frame.xh>
#endif

#ifdef _PLATFORM_OS2_
#include <MMIO.H>
#endif


//==============================================================================
// Constants
//==============================================================================
const ODPlatformType kODFileType_hfs = (ODPlatformType) 0x68667320;     // 'hfs '

//==============================================================================
// Static Function declarations
//==============================================================================

ODTime  ODGetDateProperty(Environment* ev, ODStorageUnit* su,
                        ODPropertyName prop);

void    ODSetDateProperty(Environment* ev, ODStorageUnit* su,
                          ODPropertyName prop, ODTime dateTime)

{       ODSetTime_TProp(ev, su, prop, kODTime_T, dateTime);}

//------------------------------------------------------------------------------
// SetCategory
//------------------------------------------------------------------------------

void SetCategory(ODEditor editorID, ODType  kindID,
                          HWND  categoryItem, ODSession* session)
{

    ODPartHandlerRegistryInt* ODRegistry;
    _IDL_SEQUENCE_string CategoryList;

      ODName*         name;
      ODPtr   strHandle = 0;
      ODSShort i = 0;

      Environment* ev = somGetGlobalEnvironment();
      ODRegistry = session -> GetRegistry(ev);


      // retrieve Category list from Registry
      // this is a buffer that the Registry allocates and we need to dispose of.
      CategoryList = ODRegistry-> GetCategoryList( ev, editorID, kindID);


      if (CategoryList. _length )
      {
           // Copy the Category name to dialog box and select it
            i = 0;
#if defined(_PLATFORM_WIN32_)
          SendMessage(categoryItem, LB_ADDSTRING, 0, (LPARAM)CategoryList. _buffer[i]);
          SendMessage(categoryItem, LB_SETCURSEL, (WPARAM)0, 0);
#elif defined(_PLATFORM_OS2_)
          WinSendMsg(categoryItem,  LM_INSERTITEM, MPFROMSHORT(LIT_END),
            MPFROMP(CategoryList. _buffer[i]));
          WinSendMsg(categoryItem,  LM_SELECTITEM, MPFROMSHORT(0), MPFROMSHORT(TRUE));
#elif defined(_PLATFORM_UNIX_)
         // need display call for aix implementation
#endif

      }

      // now free up the buffer we got from the Registry
      for ( i = 0; i < CategoryList. _length; i++)
      {
          SOMFree( CategoryList. _buffer[i]);
      }
      SOMFree( CategoryList. _buffer);

}

//------------------------------------------------------------------------------
// GetUserKindFromKind
//------------------------------------------------------------------------------
ODBoolean GetUserKindFromKind( ODNameSpaceManager* theNmSpcMgr,
                                                                        ODType kind, ODName** name )
{
        ODBoolean       result  = kODFalse ;

    Environment* ev = somGetGlobalEnvironment();

                // look it up in the spaceName namespace
        ODValueNameSpace* userStringNameSpace =
                (ODValueNameSpace*)theNmSpcMgr->HasNameSpace( ev, kODKindUserString );

        if (userStringNameSpace)
                result = ValueNameSpaceGetODName(userStringNameSpace, ev, kind, name );
        return result ;
}


//------------------------------------------------------------------------------
// GetUserEditorFromEditor
//------------------------------------------------------------------------------
ODBoolean GetUserEditorFromEditor( ODNameSpaceManager* theNmSpcMgr,
                                                                        ODEditor editor, ODName** name )
{
        ODBoolean       result  = kODFalse ;

    Environment* ev = somGetGlobalEnvironment();

                // look it up in the spaceName namespace
        ODValueNameSpace* userStringNameSpace =
                (ODValueNameSpace*)theNmSpcMgr->HasNameSpace( ev, kODEditorUserString );

        if (userStringNameSpace)
                result = ValueNameSpaceGetODName(userStringNameSpace, ev, editor, name );
        return result ;
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
// ChangeEditor
//------------------------------------------------------------------------------

void ChangeEditor(ODPart* part, ODEditor editorID)
{
        Environment* ev = somGetGlobalEnvironment();
        ((ODPartWrapper*)part)->UseEditor(ev, editorID);

        // Need to acquire storage unit before using it. Put it in
        // temp object so that the storage unit is released when
        // the temp object is out of scope.
        TempODStorageUnit thePartSU = part->GetStorageUnit(ev);
        thePartSU->Acquire(ev);
        if (thePartSU->Exists(ev, kODPropPreferredEditor, kODEditor, 0 ))
                thePartSU->Focus(ev, kODPropPreferredEditor,kODPosSame,kODEditor,0,kODPosSame);
        else
        {
                thePartSU->AddProperty(ev, kODPropPreferredEditor );
                thePartSU->AddValue( ev, kODEditor );
        }
        StorageUnitSetValue(thePartSU, ev, ODISOStrLength((const ODISOStr)editorID)+1,(ODValue)editorID);

}


//------------------------------------------------------------------------------
// EditorSupportsKind
//------------------------------------------------------------------------------
ODBoolean EditorSupportsKind( ODNameSpaceManager* theNmSpcMgr,
                              ODEditor editor, ODType kind )
{
    ODTypeList*     kindsList = kODNULL;
    Environment* ev = somGetGlobalEnvironment();

                // get the editorkinds namespace
        ODObjectNameSpace* editorkindsNameSpace =
                (ODObjectNameSpace*)theNmSpcMgr->HasNameSpace( ev, kODEditorKinds );

        if (editorkindsNameSpace)
        {
                        // Lookup the kinds list by editor class ID
                if (!editorkindsNameSpace->GetEntry( ev, (ODISOStr)editor,
                                                                        (ODObject**)&kindsList ))
                        kindsList = (ODTypeList*)kODNULL;
        }

        return (kindsList && kindsList->Contains(ev, kind));
}

//------------------------------------------------------------------------------
// EditorSupportsKind_Reg
//------------------------------------------------------------------------------
ODBoolean EditorSupportsKind_Reg( ODEditor editor, ODType kind,
                      ODSession* session)
{
        ODTypeList*     kindsList = kODNULL;
       _IDL_SEQUENCE_ISOString PartKindList;
        ODSShort  kind_found = FALSE;
        Environment*            ev = somGetGlobalEnvironment();
        ODPartHandlerRegistryInt* ODRegistry;
        ODSShort                i;

        // retrieve PartKind list from Registry
        // this is a buffer that the Registry allocates and we need to dispose of.
        ODRegistry = session -> GetRegistry(ev);
        PartKindList = ODRegistry-> GetPartKindList( ev, editor, 0);


        // search the PartKind list for matching kind
        for ( i = 0; i < PartKindList. _length; i++)
        {

           if (strcmp(PartKindList. _buffer[i],kind) == 0 )
           {
              // set unique to false if duplicate editor exists in string array
              kind_found = TRUE;
           }
        }

        return (kind_found);
}

//------------------------------------------------------------------------------
// ReadSize
//------------------------------------------------------------------------------

ODULong  ReadSize(ODFileRefNum file, ODStorageUnitView* view)
{
        ODULong data;
        ODSLong length = sizeof(data);
        ODSLong savedLength = length;
        Environment* ev = somGetGlobalEnvironment ();

        if (view)  // if a prefocused storage unit is passed in
                length = StorageUnitViewGetValue(view, ev, length, &data);
        else { // a file ref has been passed in

#ifdef _PLATFORM_MACINTOSH_
                THROW_IF_ERROR(FSRead(file, &length, (long *)&data));
#endif

#ifdef _PLATFORM_WIN32_
        DWORD bytesRead;
        BOOL successful;
        successful = ReadFile(file, &data, length, &bytesRead, NULL);
        if (!successful)
                THROW (GetLastError());
        length = bytesRead;
#endif  // defined _PLATFORM_WIN32_

        }
        if (savedLength != length)
                THROW(kODErrReadErr);

        return data;
}

//------------------------------------------------------------------------------
// ReadBytes
//------------------------------------------------------------------------------

void ReadBytes(ODFileRefNum file, ODStorageUnitView* view,
               ODPtr bytes, ODULong* length)
{
        ODSLong savedLength = *length;
        Environment* ev = somGetGlobalEnvironment ();

        if (view) // if a prefocused storage unit is passed in
                *length = StorageUnitViewGetValue(view, ev, *length, bytes);
        else { // a file ref has been passed in

#ifdef _PLATFORM_MACINTOSH_
                THROW_IF_ERROR(FSRead(file, (long *)length, bytes));
#endif

#ifdef _PLATFORM_WIN32_
        DWORD bytesRead;
        BOOL successful;
        successful = ReadFile(file, bytes, *length, &bytesRead, NULL);
        if (!successful)
                THROW (GetLastError());
        *length = bytesRead;
#endif // defined(_PLATFORM_WIN32_)

        }
        if (savedLength != (ODSLong)*length)
                THROW(kODErrReadErr);
}


//------------------------------------------------------------------------------
// ODGetEditorSetFromResFile
//------------------------------------------------------------------------------

EditorSet* ODGetEditorSetFromResFile(ODSShort resRefNum)
{
        EditorSet*      theEditorSet = kODNULL;
        ODBoolean       editorFound = kODFalse;

        return theEditorSet;
}


//------------------------------------------------------------------------------
// add following dunctions from the defunct InfoUtil.cpp file
// ODSetPOName        ODGetCategory      ODGetKindFromPartSU
// ODGetCreationDate  ContentValueTypes  GetThisKindFromList
// ODGetCatFromKind   ODGetDateProperty  ODGetCatFromPartSU
// ODGetSUFromPstObj
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ODSetPOName
//------------------------------------------------------------------------------
void            ODSetPOName(Environment* ev, ODPersistentObject* pstobj,ODIText* name)
{
        ODSetITextProp(ev, ODGetSUFromPstObj(ev, pstobj), kODPropName, kODMacIText, name);
}

//------------------------------------------------------------------------------
// ODGetKindFromPartSU
//------------------------------------------------------------------------------
ODType  ODGetKindFromPartSU(Environment* ev, ODStorageUnit* su)
{
   ODULong unused;
   ODType kind = ODGetISOStrProp(ev, su, kODPropPreferredKind, kODISOStr, kODNULL, &unused);
   if ( kind == kODNULL )
      if ( su->Exists(ev, kODPropContents, (ODValueType) kODNULL,1) )
      {
          su->Focus(ev, kODPropContents, kODPosSame, (ODValueType) kODNULL,1, kODPosSame);
          kind = su->GetType(ev);

          ODTranslation* translation = su->GetSession(ev)->GetTranslation(ev);
          TempODType hfsType =
                  translation->GetISOTypeFromPlatformType(ev, kODFileType_hfs, kODPlatformDataType);

          if ( ODISOStrEqual(kind, hfsType) )
          {
              ODDisposePtr(kind);
              kind = kODNULL;
              if ( su->Exists(ev, kODPropContents, (ODValueType) kODNULL,2) )
              {
                  su->Focus(ev, kODPropContents, kODPosSame, (ODValueType) kODNULL,2, kODPosSame);
                  kind = su->GetType(ev);
              }
          }
      }
   return kind;
}

//------------------------------------------------------------------------------
// ODGetCreationDate
//------------------------------------------------------------------------------
ODULong ODGetCreationDate(Environment* ev,
                                ODStorageUnit* su)
{       return ODGetDateProperty(ev, su, kODPropCreateDate);}

//------------------------------------------------------------------------------
// ContentValueTypes
//------------------------------------------------------------------------------
void ContentValueTypes(ODStorageUnit* contentSU, ODTypeList* typeList)
{
    TempODType hfsType = kODNULL;

    TRY
        ODULong         count;
        ODULong         index;

        Environment* ev = somGetGlobalEnvironment();

        ODTranslation* translation = contentSU->GetSession(ev)->GetTranslation(ev);
        hfsType = translation->GetISOTypeFromPlatformType(ev, kODFileType_hfs, kODPlatformDataType);

        contentSU->Focus(ev, kODPropContents, kODPosUndefined, 0, 0, kODPosUndefined);
        count = contentSU->CountValues(ev);
        for (index = 1; index <= count; ++index)
        {
            contentSU->Focus(ev, kODPropContents, kODPosUndefined, 0, index, kODPosUndefined);
            TempODType type = contentSU->GetType(ev);
            if ( !ODISOStrEqual(type, hfsType) )
                    typeList->AddLast(ev, type);
        }
    CATCH_ALL
    ENDTRY
}

//------------------------------------------------------------------------------
// GetThisKindFromList
//------------------------------------------------------------------------------
ODType GetThisKindFromList(ODSShort kindItem, ODTypeList* kindList)
{
        Environment* ev = somGetGlobalEnvironment();
        ODType type = kODNULL;
        ODTypeListIterator* kindIter = kindList->CreateTypeListIterator(ev);

        for ( type = kindIter->First(ev);
                  (--kindItem > 0) && kindIter->IsNotComplete(ev);
                  type = kindIter->Next(ev) )
        {
                ODDisposePtr(type);
                type = kODNULL;
        }

        ODDeleteObject(kindIter);

        return type;
}

//------------------------------------------------------------------------------
// ODGetCatFromPartSU
//------------------------------------------------------------------------------
ODName* ODGetCatFromPartSU(Environment* ev, ODStorageUnit* su, ODNameSpaceManager* nsm)
{

   ODSShort      i;
   _IDL_SEQUENCE_string CategoryList;
   ODPartHandlerRegistryInt* ODRegistry;
   ODName* category = kODNULL;

       ODType kindID = ODGetKindFromPartSU(ev, su);
       ODSession* session = su->GetSession(ev);
       ODRegistry = session -> GetRegistry(ev);


       // retrieve Category list from Registry
       // this is a buffer that the Registry allocates and we need to dispose of.

       CategoryList = ODRegistry-> GetCategoryList( ev, 0, kindID);

       if (CategoryList. _length )
       {
          i = 0;
          // Set default Category type when NO part name was supplied by part

          category=StrToIntl((PSZ)CategoryList. _buffer[i], kODNULL);

          // now free up the buffer we got from the Registry
          for ( i = 0; i < CategoryList. _length; i++)
          {
              SOMFree( CategoryList. _buffer[i]);
          }
          SOMFree( CategoryList. _buffer);

       }

       ODDisposePtr( kindID );

       return category;
}

//------------------------------------------------------------------------------
// ODGetSUFromPstObj
//------------------------------------------------------------------------------
ODStorageUnit* ODGetSUFromPstObj(Environment* ev, ODPersistentObject* pstobj)
{
        return pstobj->GetStorageUnit(ev);
}

//------------------------------------------------------------------------------
// ODGetDateProperty - static declaration above
//------------------------------------------------------------------------------
ODTime  ODGetDateProperty(Environment* ev,
                        ODStorageUnit* su,
                        ODPropertyName prop)
{       return ODGetTime_TProp(ev, su, prop, kODTime_T);}

//--------------------------------------------------------------------
//  IntlToStr
//--------------------------------------------------------------------

PSZ IntlToStr(ODIText* intlText, PSZ pstr)
{


        if (pstr == NULL) return pstr;

        if (intlText == kODNULL) {
           *pstr = '\0';
            return pstr;
           }

        PSZ   tmp = GetCStringFromIText(intlText);

        if (strlen(tmp) == 0)
           *pstr = '\0';
        else {
           strcpy(pstr, tmp);
           }

        return pstr;
        }

//--------------------------------------------------------------------
// StrToIntl
//--------------------------------------------------------------------
ODIText* StrToIntl(PSZ pstr, ODIText** intlText)
{

        ODIText* result = kODNULL;

        if ( intlText )
           result = *intlText ;

        if ( !result )
           {
           result = CreateIText(0, 0, pstr);
           }
        else
           {
           SetITextScriptCode(result, 0);
           SetITextLangCode(result, 0);
           SetITextString(result, pstr);
           }

        if ( intlText )
           *intlText = result ;

        return result;
}

ODULong ODGetModificationDate(Environment* ev,
                                ODStorageUnit* su)
{       return ODGetDateProperty(ev, su, kODPropModDate);}

void    ODSetModificationDate(Environment* ev,
                                ODStorageUnit* su,
                                ODTime dateTime)
{       ODSetDateProperty(ev, su, kODPropModDate, dateTime);}

ODIText*        ODGetPOName(Environment* ev, ODPersistentObject* pstobj,ODIText* name)
{
        return ODGetITextProp(ev, ODGetSUFromPstObj(ev, pstobj), kODPropName, kODMacIText,name);
}


void            ODSetPONameUsingSU(Environment* ev, ODStorageUnit* su, ODIText* name)
{
        ODSetITextProp(ev, su, kODPropName, kODMacIText, name);
}

ODIText*        ODGetModifiedBy(Environment* ev,
                                ODStorageUnit* su, ODIText* userName)
{
        return ODGetITextProp(ev, su, kODPropModUser,
                        kODMacIText, userName);
}

void            ODSetModifiedBy(Environment* ev,
                                ODStorageUnit* su, ODIText* userName)
{
        ODSetITextProp(ev, su, kODPropModUser,
                        kODMacIText, userName);
}

ODIText*
ODGetPOComments(Environment* ev, ODPart *part, ODIText* comments)
{
        ODStorageUnit*  su = ODGetSUFromPstObj(ev, part);
        ODIText*                propComments;

        propComments = ODGetITextProp(ev, su, kODPropComments, kODMacIText, comments);
        return propComments;
}

void    ODSetPOComments(Environment* ev, ODPart *part, ODIText* comments)
{
        ODStorageUnit*  su = ODGetSUFromPstObj(ev, part);

        ODSetITextProp(ev, su, kODPropComments, kODMacIText, comments);
}

//--------------------------------------------------------------------
// ODGetIsStationery
//--------------------------------------------------------------------

ODBoolean       ODGetIsStationery(Environment* ev,
                                ODFrame* frame)
{
//  ODBoolean    isStationery;
//  TempPlatformFile usersFile = ODGetFileIfRoot(ev, frame);
//  if (usersFile)
//          isStationery = usersFile->IsStationery();

    TempODPart              part = frame->AcquirePart(ev);
    TempODFrame  containingFrame = frame->AcquireContainingFrame(ev);
    ODBoolean         isRootPart = ( containingFrame == kODNULL );
    TempODStorageUnit         su = part->GetStorageUnit(ev);
    ODBoolean stationeryExist = kODFalse;

    // Need to acquire storage unit before using it
    su->Acquire(ev);

    if (isRootPart)
    {

       // kODPropIsStationery is a property to tell whether
       // this is a stationery or not.
       // Check whether this property exists or not. If this
       // property exists, get the value.
       stationeryExist = (ODSUExistsThenFocus(ev, su,
                                           kODPropIsStationery,
                                           kODBoolean));

       if (stationeryExist)
           StorageUnitGetValue(su, ev, sizeof(ODBoolean),
                                           (ODValue)&stationeryExist);


    }

    return stationeryExist;

}

//--------------------------------------------------------------------
// ODSetIsStationery
//--------------------------------------------------------------------

void            ODSetIsStationery(Environment* ev,
                                  ODFrame* frame,
                                  ODBoolean isStationery)
{
//      { TempODPart part = frame->AcquirePart(ev);
//        ODSetBooleanProp( ev, ODGetSUFromPstObj(ev, part),
//                          kODPropIsStationery, kODBoolean, isStationery);
//      }
//      TempPlatformFile usersFile = ODGetFileIfRoot(ev, frame);
//      if (usersFile)
//              usersFile->SetStationery(isStationery);
    TempODPart              part = frame->AcquirePart(ev);
    TempODFrame  containingFrame = frame->AcquireContainingFrame(ev);
    ODBoolean         isRootPart = ( containingFrame == kODNULL );


    if (isRootPart)
    {

    // Set kODPropIsStationery property to signal that this file
    // is a stationery
        TempODStorageUnit    su = part->GetStorageUnit(ev);
        su->Acquire(ev);
        ODSetBooleanProp(ev, su, kODPropIsStationery, kODBoolean, isStationery);
    }

}

ODType  ODGetKind(Environment* ev, ODPart* part)
{
        return ODGetKindFromPartSU(ev, ODGetSUFromPstObj(ev, part));
}

ODULong ODGetPOSize(Environment* ev, ODPersistentObject* pstobj)
{
        ODStorageUnit* su = ODGetSUFromPstObj(ev, pstobj);
        su->Focus(ev, kODNULL, kODPosAll, kODNULL,0, kODPosAll);
        return su->GetSize(ev);
}

