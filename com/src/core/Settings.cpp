/* @(#)Z 1.5 os2/src/core/Settings.cpp, odcore, od96os2, odos29646d 96/11/15 15:53:53 (96/09/30 14:49:08) */
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

/*
        File:           Settings.cpp

        Contains:       stub code for the ODSettingsExtension class

        Owned by:       Tantek ‚elik

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#define ODSettingsExtension_Class_Source
#define VARIABLE_MACROS
#include <ODTypes.h>
#include <Settings.xih>

//==============================================================================
// ODSettingsExtension
//==============================================================================

SOM_Scope void  SOMLINK ODSettingsExtensionInitSettingsExtension
                (ODSettingsExtension *somSelf, Environment *ev,
                ODPart* owner, ODULong page_count, ODFacet* facet)
{
    ODSettingsExtensionData *somThis =
                ODSettingsExtensionGetData(somSelf);
    ODSettingsExtensionMethodDebug("ODSettingsExtension",
                "ODSettingsExtensionInitSettingsExtension");

        SOM_TRY
        somSelf->InitExtension(ev, (ODObject*) owner);


        //initialize the number sheets contained in resource array
        _fsheet_count = 0;

        //initialize the number of count for sheets to be inserted into notebook
        _fnumpagesneeded = page_count;  //initialize



        SOM_CATCH_ALL
        SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODSettingsExtensionShowSettings
                (ODSettingsExtension *somSelf, Environment *ev,
                ODFacet* facet)
{
    /* ODSettingsExtensionData *somThis =
                ODSettingsExtensionGetData(somSelf); */
    ODSettingsExtensionMethodDebug("ODSettingsExtension",
                "ODSettingsExtensionShowSettings");


}

SOM_Scope void  SOMLINK ODSettingsExtensionsomUninit
                (ODSettingsExtension *somSelf)
{
    /* ODSettingsExtensionData *somThis =
                ODSettingsExtensionGetData(somSelf); */
    ODSettingsExtensionMethodDebug("ODSettingsExtension",
                "ODSettingsExtensionsomUninit");

    ODSettingsExtension_parent_ODExtension_somUninit(somSelf);
}

SOM_Scope void  SOMLINK ODSettingsExtensionRelease
                (ODSettingsExtension *somSelf, Environment *ev)
{
    /* ODSettingsExtensionData *somThis =
                ODSettingsExtensionGetData(somSelf); */
    ODSettingsExtensionMethodDebug("ODSettingsExtension",
                "ODSettingsExtensionRelease");

        SOM_TRY

            ODSettingsExtension_parent_ODExtension_Release(somSelf,ev);

        SOM_CATCH_ALL
        SOM_ENDTRY
}


SOM_Scope void  SOMLINK ODSettingsExtensionGetNotebookSheets
                (ODSettingsExtension *somSelf, Environment *ev,
                 ODSheet** Sheet, ODULong* sheet_count)

{
    ODSettingsExtensionData *somThis =
                ODSettingsExtensionGetData(somSelf);
    ODSettingsExtensionMethodDebug("ODSettingsExtension",
                "ODSettingsExtensionGetNotebookSheets");

    // get array of part's property sheet resources & sheet count
    *Sheet = _fSheet ;
    // post increment of 0 based sheet array keeps count in sync
    *sheet_count = _fsheet_count;

}

SOM_Scope void SOMLINK ODSettingsExtensionAddNotebookSheet
               (ODSettingsExtension *somSelf, Environment *ev,
                PFNWP  dlg_proc, HMODULE hMod, ULONG dlg_id)

{
    ODSettingsExtensionData *somThis =
                ODSettingsExtensionGetData(somSelf);
    ODSettingsExtensionMethodDebug("ODSettingsExtension",
                "ODSettingsExtensionAddNotebookSheet");


    _fSheet[_fsheet_count].dlgproc = dlg_proc;
    _fSheet[_fsheet_count].hmod = hMod;
    _fSheet[_fsheet_count].id = dlg_id;

    _fsheet_count++;

}
