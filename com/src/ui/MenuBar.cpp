/* @(#)Z 1.56 com/src/ui/MenuBar.cpp, odui, od96os2, odos29646d 96/11/15 15:28:06 (96/10/23 15:18:07) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odui
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
        File:           MenuBar.cpp

        Contains:       Implementation of ODMenuBar

        Owned by:       Richard Rodseth

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

        IBM Change History (most recent first):
  Date      Body  Defect  Reason
  --------  ----  ------  -----------------------------------------------
  07/10/96   brr          Not updating new positions correctly in AddMenuIDs
                           This is called BEFORE the new menu is added not
                           AFTER, therefore, don't add 1 to the position in
                           the loop to start off with.
  07/10/96   brr          Check to see that the id of the item being added
                           through AddMenuItemBefore and AddMenuItemLast is
                           a unique ID before adding it to the menu.
  07/08/96   brr   23720  Return kODTrue in InsertSubMenu for NT if it worked
                          Use menuID if no subMenuID when adding the new
                           submenu.
                          When AppendMenu is used to do InsertSubMenu, need
                           to use ParentHandle not menuInfo->handle as the
                           handle to the menu.
  07/03/96   brr   23697  Return kODTrue in GetMenuItem for NT if it worked
  07/03/96   brr   23696  Have DeleteMenuIDs recurse if deleting a child that
                           is a submenu itself
  07/02/96   brr   23672  Take out all use of save_hWindow
                          Take out dead code at bottom of file
                          For OS/2 change checking results for MM_INSERTITEM to
                           allow 0 to be a successful rc.
  07/01/96   brr   23645  Changes to InsertSubMenuEx to handle replacing an
                           existing submenu with a new submenu.
  06/28/96   brr   23645  Need to use beforeMenuInfo not miBefore to get
                           position for menu item being added in AddMenuItemBefore
  06/27/96   brr   23112  Copy AccelTable when copying a Menu
  06/26/96   brr   23551  Add use of saved_hWindow in some of the routines for
                           OS/2
  06/25/96   brr   23492  Use GetWindowSystemData for OS/2 now supported
                          Check for 0 submenu id's in Copy before trying to
                           access array
                          Change CheckForValidIDs to explicitly check to see
                           if it is a submenu - without relying on -1 id only
                          Change AddSubMenuIDs to explicitly check to see
                           if it is a submenu - without relying on -1 id only
                          Fix bugs in OS/2 portion of CopyMenu - getting wrong
                           id for submenu because using wrong handle for lookup
                          Changed reliance on 0xFFFFFFFF for identifying
                           submenus to using actual submenu handles instead in
                           InsertSubMenuEx and AddMenuItemIDBefore
  06/24/96   brr   23451  Return kODFalse if can't find menuid in EnableMenuItem
  06/14/96   brr   23056  Add SetupCopy private method and make CountSubMenus
                           a private method to support Copy for Popup's
  06/13/96   brr          Add comments
  06/06/96   brr   22794  Take out use of GetWindowSystemData for OS/2 - not
                           supported yet on OS/2 platform
  06/05/96   brr   22722  Use GetWindowSystemData to get handle to resource
  06/04/96   brr   21795  Check if item being removed is a Popup or item in
                           RemoveMenuItem - if popup, then call RemoveMenu
                           instead.
  06/04/96   brr   22656  Have GetSubMenuHandle call GetSubMenu NOT GetSubMenuHandle
  05/08/96   brr   21758  Delete the following API's because not used:
                           ReplaceSubmenu
                           SetNothingAvailable
                           SetShowFrameOutline
                           CheckMenuBarID
                           AppendOLEMenuToODMenuBar
                           InitSelectedMenuBar
                           CopySelectedMenu
                           AddSelectedMenu
                           RestoreSelectedMenu
                           AddSelectedToAccelTable
                           AddSelAccels
  05/07/96   brr   21712  Add AddMenuBeforeEx and AddMenuLastEx and InsertSubmenuEx
  04/24/96   brr   21392  Fixes for all subMenuIDarray stuff and AddMenuItemBefore
  04/22/96   brr   21316  Changed new param to InitMenuBar to be an
                           address to an array of submenu ID's rather
                           than the menuID we had tried to do with
                           defect #21263
  04/19/96   brr   21263  Added SetAccelTable function and modified InitMenuBar
  04/16/96   brr          Add support for fmenuIDmap
  04/15/96   brr          Change logic for sizeof(ODACCEL) to take into account
                           that the array of accel[1] not [0] in the definition.
  04/12/96   brr          Fix CopyAccelTable so that TempTable->cAccel is set
                           to ulCols.
  04/12/96   brr          Do a SetMenu instead of SetParent in Display
  04/09/96   brr   20925  Get rid of the SomFree for MenuString since it is
                           created on the local stack
                          Make sure that the somUninit checks the _Head to
                           free it if it is not NULL.
                          Rearrange the code, so that the common API's are
                           together so that we don't have 2 instances of the
                           same API in different places in the code.
  04/05/96   brr   20861  Change _PLATFORM_WIN32 to _PLATFORM_WIN32_
  03/28/96   brr   20780  Added ODMenuBarGetSystemMenu and ODMenuBarGetAccelTable
  03/28/96   brr   20780  Changed HACCEL types to ODPlatformAccelTable
  03/12/96   brr   20109  Removed references to MS_ACTIONBAR - appears
                           Windows does not have the concept of Menu Styles
                          Removed references to MS_SAVEBITS - appears
                           Windows does not support this performance feature
                          When getting/setting the window identity, use
                           Get/SetWindowLong instead of Get/SetWindowWord
                           (as done in OS/2) since windows gets the id
                           (GWL_ID vs. QWS_ID) with the Long API.
                         Added define for NULLHANDLE for WIN32.
  02/29/96   brr   20109  Added WIN32 implementation support

        Change History (most recent first):

    <27>    10/23/95        RR              #1291688 Synch with menu manager when
                                                      adding & removing menus, if this menu bar
                                                      is the current menu bar
    <26>     10/8/95        TJ              Fixes Recomended by Refball
    <25>     9/12/95        eeh             1283557: use SetErrorCode
    <24>     8/21/95        RR              #1271358 GetMenuAndItem throws
                                                      kODErrInvalidCommandID. EnableCommand etc.
                                                      suppress this error and fail silently
    <23>     8/16/95        NP              1274946: ErrorDef.idl problems. Add include
                                                      file.
    <22>     8/15/95        RR              #1276683 remove extra new ODPrivMenuInfo.
    <21>      8/3/95        RR              #1257260 Collapse B classes. Remove
                                                      somInit methods. Don't call IsInitialized
                                                      or SubclassResponsibility
    <20>     7/25/95        VL              #1270320 Delete foundMenu in RemoveMenu.
                                                      Delete elements in _fOtherMenus before
                                                                        deleting it.
    <19>     7/25/95        eeh             #1259218 remove assert in SetItemString
    <18>      6/8/95        RR              #1257260 Collapse base classes.
                                            #1214898 StdTypes.r -> ODTypes.r
    <17>     5/26/95        RR              #1251403 Multithreading naming support
    <16>     5/17/95        RR              #1250135/1250137/1250143 Getters increment refcount
    <15>     4/28/95        RR              #1211085 Remove 5$ comments
    <14>     4/13/95        RR              #1216618 Added ODVolatile,
                                            #1220325 Removed private error code
    <13>      4/7/95        RR              #1216618 Added SOM_TRY etc.
    <12>      4/6/95        RR              #1211085 Remove 5$ comments
    <11>      4/4/95        RR              #1197749 Get/Set ItemString deal with script code
    <10>     3/22/95        RR              #1197749, 1194296 Set script code in SetItemString
     <9>     1/23/95        RR              # 1212896 Added EnableAll and DisableAll
     <8>     1/12/95        jpa             Don't use obsolete Toolbox names [1211211]
     <7>     9/29/94        RA              1189812: Mods for 68K build.
     <6>     8/25/94        RR              Implemented IsValid. Added private
                                             SetGeneration/GetGeneration
     <5>     8/15/94        JBS             #1181023 return kODTrue in IsValid
     <4>     8/15/94        CG              #1181023 Added new IsValid API.
                                                      It is not implemented yet.
     <3>     7/19/94        eeh             fix to ODMenuBarCopy
     <2>      7/6/94        RR              Implemented SetItemString and GetItemString
     <1>     6/23/94        RR              first checked in

        To Do:
*/

#ifndef SOM_Module_menubar_Source
#define SOM_Module_menubar_Source
#endif

#define ODMenuBar_Class_Source
#define VARIABLE_MACROS
#include "MenuBar.xih"

#include <ODSessn.xh>
#include <Window.xh>
#include <WinStat.xh>

#include <OrdColl.h>
#include <ODUtils.h>

#ifdef _PLATFORM_OS2_
#include <ODos2.h>
#include <odres.h>
#include <MenuAdO.xh>
#endif // PLATFORM_OS2

#ifdef _PLATFORM_WIN32_
#include <odres.h>
#include <olemgrw.h>
#include <MenuAdW.xh>
#define NULLHANDLE ((unsigned long) 0)   // defined in os2defs.h for OS/2
#endif // PLATFORM_WIN32

#ifdef _PLATFORM_AIX_
#include <odres.h>
#include <MenuAdX.xh>
#define NULLHANDLE ((unsigned long) 0)   // defined in os2defs.h for OS/2
#endif // PLATFORM_AIX

//=====================================================================================
// LOCAL NON-SOM METHODS
//=====================================================================================

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
//-------------------------------------------------------------------------------------
// CopyAccelTable [local non-SOM] [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
ODPlatformAccelTable CopyAccelTable(ODPlatformAccelTable sourceTable)
{

    ODULong ulSize = 0;
    ODULong ulCols = 0;
    ODACCELTABLE *TempTable	  = kODNULL;
    ODPlatformAccelTable NewTable = kODNULL;

    // copy our source table to a temporary table
    // ------------------------------------------
    #ifdef _PLATFORM_OS2_
	ulSize = WinCopyAccelTable(sourceTable, NULL, 0);
        ODULong tmpSize = (sizeof(ODACCELTABLE) + ulSize - sizeof(ACCEL));
	TempTable = (ODACCELTABLE*)malloc(tmpSize);
	WinCopyAccelTable(sourceTable, (PACCELTABLE)TempTable, ulSize);
	NewTable = WinCreateAccelTable(WinQueryAnchorBlock(HWND_DESKTOP),
					(PACCELTABLE)TempTable);
    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ulCols = CopyAcceleratorTable(sourceTable, NULL, 0);
	ulSize = ulCols * sizeof(ACCEL);
        ODULong tmpSize = (sizeof(ODACCELTABLE) + ulSize - sizeof(ACCEL));
	TempTable = (ODACCELTABLE*)malloc(tmpSize);
	TempTable->cAccel = ulCols;
	CopyAcceleratorTable(sourceTable, TempTable->aaccel, TempTable->cAccel);
	NewTable = CreateAcceleratorTable(TempTable->aaccel, ulCols);
    #endif //PLATFORM_WIN32

    // free up the memory we allocated
    // -------------------------------
    if (TempTable)
	free(TempTable);

    return(NewTable);

} /* end CopyAccelTable */

#endif // PLATFORM_OS2, PLATFORM_WIN32, PLATFORM_AIX

#ifdef _PLATFORM_OS2_
//-------------------------------------------------------------------------------------
// GetOS2MenuAdapter [local non-SOM] [OS/2 only]
//-------------------------------------------------------------------------------------
ODBoolean GetOS2MenuAdapter(ODMenuBar* somSelf,
			    Environment* ev,
			    ODMenuID menu,
			    ODOS2MenuAdapter** os2menuAd)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);

    ODBoolean found = kODFalse;

    if (menu == ID_BASEMENUBAR)	// not gonna find it!
	return kODFalse;

    if (menu == IDMS_HELP)	// if it exists, it'll be last
    {
	if (! somThis->fHelpMenuExists)
	    return kODFalse;

	*os2menuAd = (ODOS2MenuAdapter*) _fMenus->Last();
	    return kODTrue;
    }

    OrderedCollectionIterator iter(_fMenus);
    for (ODOS2MenuAdapter* menuAd = (ODOS2MenuAdapter*) iter.First();
	 ((! found) && (iter.IsNotComplete()));
	 menuAd = (ODOS2MenuAdapter*) iter.Next())
    {
	if (menuAd->MenuID(ev) == menu)
	{
	    found	= kODTrue;
	    *os2menuAd  = menuAd;
	}
	else {
	    ODOS2MenuAdapter* subMenuAd;
	    if (menuAd->GetSubMenuAdapter(ev, menu, &subMenuAd))
	    {
		found	   = kODTrue;
		*os2menuAd = subMenuAd;
		
	    }
	}
    }

    return found;

} /* end GetOS2MenuAdapter */

#endif // PLATFORM_OS2

#ifdef _PLATFORM_WIN32_
//-------------------------------------------------------------------------------------
// GetWinMenuAdapter [local non-SOM] [Windows only]
//-------------------------------------------------------------------------------------
ODBoolean GetWinMenuAdapter(ODMenuBar* somSelf,
			    Environment* ev,
			    ODMenuID menu,
			    ODWinMenuAdapter** winMenuAd)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);

    ODBoolean found = kODFalse;

    if (menu == ID_BASEMENUBAR)	// not gonna find it!
	return kODFalse;

    if (menu == IDMS_HELP)	// if it exists, it'll be last
    {
	if (! somThis->fHelpMenuExists)
	    return kODFalse;

	*winMenuAd = (ODWinMenuAdapter*) _fMenus->Last();
	    return kODTrue;
    }

    OrderedCollectionIterator iter(_fMenus);
    for (ODWinMenuAdapter* menuAd = (ODWinMenuAdapter*) iter.First();
	 ((! found) && (iter.IsNotComplete()));
	 menuAd = (ODWinMenuAdapter*) iter.Next())
    {
	if (menuAd->MenuID(ev) == menu)
	{
	    found	= kODTrue;
	    *winMenuAd  = menuAd;
	}
	else {
	    ODWinMenuAdapter* subMenuAd;
	    if (menuAd->GetSubMenuAdapter(ev, menu, &subMenuAd))
	    {
		found	   = kODTrue;
		*winMenuAd = subMenuAd;
		
	    }
	}
    }

    return found;

} /* end GetWinMenuAdapter */

//-------------------------------------------------------------------------------------
// GetSubMenuIDs [local non-SOM] [Windows only]
//-------------------------------------------------------------------------------------
ODBoolean GetSubMenuIDs(ODMenuBar*     somSelf,
			Environment*   ev,
			ODUShort*      nSubMenuIDs,
			ODMenuIDInfo** subMenuIDs)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);

    ODBoolean bResult   = kODTrue;

    OrderedCollection* subMenuIDcoll = new OrderedCollection;

    OrderedCollectionIterator iter(_fMenus);
    for (ODWinMenuAdapter* menuAd = (ODWinMenuAdapter*) iter.First();
	 ((bResult == kODTrue) && (iter.IsNotComplete()));
	 menuAd = (ODWinMenuAdapter*) iter.Next())
    {
	bResult = menuAd->GetSubMenuIDInfo(ev, subMenuIDcoll);
    }

    if (bResult == kODTrue)
    {
	ODUShort      count = subMenuIDcoll->Count();
        ODMenuIDInfo* info  = new ODMenuIDInfo[count];
	ODUShort      index = 0;

	while (subMenuIDcoll->Count())
	{
	    ODMenuIDInfo* nextID = (ODMenuIDInfo*)subMenuIDcoll->First();
	    subMenuIDcoll->Remove((ElementType) nextID);

	    info[index].hMenu = nextID->hMenu;
	    info[index].id    = nextID->id;
	    index++;

	    delete nextID;
	}

	delete subMenuIDcoll;

	*nSubMenuIDs = count;
	*subMenuIDs  = info;
    }

    return bResult;

} /* end GetSubMenuIDs */

//-------------------------------------------------------------------------------------
// UpdateWinMenuBar [local non-SOM] [Windows only]
//-------------------------------------------------------------------------------------
void UpdateWinMenuBar(ODMenuBar *somSelf, Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);

    ODWindowState* winStat	= kODNULL;
    ODWindow* activeWindow	= kODNULL;
    ODPlatformWindow hwndFrame	= NULL;
    HMENU activeMenu		= NULL;

    if (_fSession)
	winStat = _fSession->GetWindowState(ev);

    if (winStat)
	activeWindow = winStat->AcquireActiveWindow(ev);

    if (activeWindow)
    {
	hwndFrame = activeWindow->GetPlatformWindow(ev);
	activeWindow->Release(ev);
    }

    if (hwndFrame)
	activeMenu = GetMenu(hwndFrame);

    if (activeMenu == _fPlatMenuBar)
    {
	BOOL  res = DrawMenuBar(hwndFrame);
	DWORD err;

	if (res == FALSE)
	    err = GetLastError();
    }

} /* end UpdateWinMenuBar */

#endif // PLATFORM_WIN32

#ifdef _PLATFORM_AIX_
//-------------------------------------------------------------------------------------
// GetX11MenuAdapter [local non-SOM] [AIX only]
//-------------------------------------------------------------------------------------
ODBoolean GetX11MenuAdapter(ODMenuBar* somSelf,
			    Environment* ev,
			    ODMenuID menu,
			    ODX11MenuAdapter** aixmenuAd)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);

    ODBoolean found = kODFalse;

    if (menu == ID_BASEMENUBAR)	// not gonna find it!
	return kODFalse;

    if (menu == IDMS_HELP)	// if it exists, it'll be last
    {
	if (! somThis->fHelpMenuExists)
	    return kODFalse;

	*aixmenuAd = (ODX11MenuAdapter*) _fMenus->Last();
	    return kODTrue;
    }

    OrderedCollectionIterator iter(_fMenus);
    for (ODX11MenuAdapter* menuAd = (ODX11MenuAdapter*) iter.First();
	 ((! found) && (iter.IsNotComplete()));
	 menuAd = (ODX11MenuAdapter*) iter.Next())
    {
	if (menuAd->MenuID(ev) == menu)
	{
	    found	= kODTrue;
	    *aixmenuAd  = menuAd;
	}
	else {
	    ODX11MenuAdapter* subMenuAd;
	    subMenuAd = (ODX11MenuAdapter*) (menuAd->GetSubMenu(ev, menu));

	    if (subMenuAd)
	    {
		found	   = kODTrue;
		*aixmenuAd = subMenuAd;
	    }
	}
    }

    return found;

} /* end GetX11MenuAdapter */

#endif // PLATFORM_AIX

//=====================================================================================
// COMMON METHODS FOR ALL PLATFORMS
//=====================================================================================

//-------------------------------------------------------------------------------------
// Display
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarDisplay(ODMenuBar *somSelf, 
                                          Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarDisplay");

    ODWindow* activeWin = kODNULL;

    try {

	ODWindowState* winStat = _fSession->GetWindowState(ev);
	THROW_IF_NULL(winStat, kODErrUndefined);

        activeWin = winStat->AcquireActiveWindow(ev);
	THROW_IF_NULL(activeWin, kODErrUndefined);

    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
	HWND hwndFrame = (HWND) activeWin->GetPlatformWindow(ev);
    #endif // PLATFORM_OS2, PLATFORM_WIN32

    #ifdef _PLATFORM_OS2_

	HWND oldMenu = WinWindowFromID(hwndFrame, FID_MENU);

	if (oldMenu)
	{
	    WinSetParent(oldMenu, HWND_OBJECT, FALSE);
	    WinSetOwner (oldMenu, HWND_OBJECT);
	}

	WinSetParent(_fPlatMenuBar, hwndFrame, FALSE);
	WinSetOwner (_fPlatMenuBar, hwndFrame);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_

	BOOL  res = TRUE;
	DWORD err;

        // If running as OLE server, don't display in non-root frame;
	// and for non-root frame, display in frame's parent.
	// -----------------------------------------------------------
	if (_fSession->GetOleManager(ev)->IsOleServer())
	{
	    if (activeWin->IsRootWindow(ev))
		res = SetMenu(GetParent(hwndFrame), _fPlatMenuBar);
        }
        else
	{
            res = SetMenu(hwndFrame, _fPlatMenuBar);
	}

	if (res == FALSE)
	    err = GetLastError();

	UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_MACINTOSH_

	if (_fSystemMenus)
	{
	    ClearMenuBar();
	    SetMenuBar(_fSystemMenus);
	}

	if (_fOtherMenus)
	{
	    OrderedCollectionIterator iter(_fOtherMenus);
	    for (ODPrivMenuInfo* menu = (ODPrivMenuInfo*) iter.First();
		 iter.IsNotComplete();
		 menu = (ODPrivMenuInfo*) iter.Next())
	    {
		InsertMenu(menu->fMenu,menu->fPosition);
	    }
	}

	InvalMenuBar();

    #endif // _PLATFORM_MACINTOSH

    #ifdef _PLATFORM_AIX_

	Widget mainWinWidget    = activeWin->GetMainWindowWidget(ev);
	Widget oldMenuBarWidget = NULL;
	Widget newMenuBarWidget = NULL;

	XtVaGetValues(mainWinWidget, XmNmenuBar, &oldMenuBarWidget, NULL);

	newMenuBarWidget = XmCreateMenuBar(mainWinWidget, "", NULL, 0);

	if (_fMenus)
	{
	    OrderedCollectionIterator iter(_fMenus);
	    for (ODX11MenuAdapter* menuAd = (ODX11MenuAdapter*) iter.First();
		 iter.IsNotComplete();
		 menuAd = (ODX11MenuAdapter*) iter.Next())
	    {
		menuAd->DisplayMenu(ev, newMenuBarWidget);
	    }
	}

	XtManageChild(newMenuBarWidget);
	XtVaSetValues(mainWinWidget, XmNmenuBar, newMenuBarWidget, NULL);

	if (oldMenuBarWidget)
	    XtDestroyWidget(oldMenuBarWidget);

    #endif // PLATFORM_AIX

    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        somSelf->SetAccelTableToWindow(ev);
    #endif // PLATFORM_OS2, PLATFORM_WIN32, PLATFORM_AIX

	winStat->SetCurrentMenuBar(ev,somSelf);

    }
    catch (ODException _exception)
    {
        SetErrorCode(kODNoError);
    }

    if (activeWin)
	activeWin->Release(ev);

} /* end Display */

//-------------------------------------------------------------------------------------
// Copy
//-------------------------------------------------------------------------------------
SOM_Scope ODMenuBar*  SOMLINK ODMenuBarCopy(ODMenuBar *somSelf, 
                                             Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarCopy");

    ODMenuBar* newMenuBar = kODNULL;

    try {

	newMenuBar = new ODMenuBar;
	THROW_IF_NULL(newMenuBar, kODErrUndefined);

    #if defined(_PLATFORM_OS2_) || defined (_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)

	// for AIX and OS/2, InitMenuBar can automatically copy
	// all the menu items and adapters, based upon the info
	// availabled in this menu bar...
	// ----------------------------------------------------
	#ifdef _PLATFORM_AIX_
	    newMenuBar->InitMenuBar(ev, _fSession, somSelf);
	#endif // PLATFORM_AIX

	#ifdef _PLATFORM_OS2_
	    newMenuBar->InitMenuBar(ev, _fSession, _fPlatMenuBar);
	#endif // PLATFORM_OS2

	// For NT, we have to retrieve the menu ID info stored
	// in our menu adapters, and then call InitMenuBarEx().
	// ----------------------------------------------------
	#ifdef _PLATFORM_WIN32_
	    ODUShort	  nSubMenuIDs = 0;
	    ODMenuIDInfo* subMenuIDs  = kODNULL;

	    if (GetSubMenuIDs(somSelf, ev, &nSubMenuIDs, &subMenuIDs))
	    {
		newMenuBar->InitMenuBarEx(ev, _fSession, _fPlatMenuBar,
					  nSubMenuIDs, subMenuIDs);
		delete subMenuIDs;
	    }
	#endif // PLATFORM_WIN32

	if (_fNewAccelTable) {
	    newMenuBar->SetAccelTable(ev, CopyAccelTable(_fNewAccelTable));
	} else {
	    newMenuBar->SetAccelTable(ev, CopyAccelTable(_fDefaultAccelTable));
	}

	newMenuBar->SetGeneration(ev, _fGeneration);

    #endif // PLATFORM_OS2, PLATFORM_WIN32, PLATFORM_AIX

    #ifdef _PLATFORM_MACINTOSH_
	newMenuBar->InitMenuBar(ev, _fSession, _fSystemMenus);


	if (_fOtherMenus)
	{
	    OrderedCollectionIterator iter(_fOtherMenus);
	    for (ODPrivMenuInfo* menu = (ODPrivMenuInfo*) iter.First();
		 iter.IsNotComplete();
		 menu = (ODPrivMenuInfo*) iter.Next())
	    {
		if (menu->fPosition == 0)
		    newMenuBar->AddMenuLast(ev, menu->fMenuID, menu->fMenu, menu->fPart);
		else if (menu->fPosition == hierMenu)
		    newMenuBar->AddSubMenu(ev, menu->fMenuID, menu->fMenu, menu->fPart);
		else
		    newMenuBar->AddMenuBefore(ev, menu->fMenuID, menu->fMenu,
						  menu->fPart, menu->fPosition);
	    }

	    if (_fCommandTable) //!!! eeewh!!!
	    {
		// newMenuBar->fCommandTable = _fCommandTable->Copy();

		newMenuBar->SetCommandTable(ev, _fCommandTable->Copy() );
		// <eeh> was somSelf->SetCommandTable
	    }
         }
	 newMenuBar->SetGeneration(ev, _fGeneration);

    #endif // PLATFORM_MACINTOSH

    }
    catch (ODException _exception)
    {
	ODDeleteObject(newMenuBar); 
        SetErrorCode(kODNoError);
    }

    return newMenuBar;

} /* end Copy */

//-------------------------------------------------------------------------------------
// AddMenuLast
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarAddMenuLast(ODMenuBar *somSelf, 
                                              Environment *ev, 
                                             ODMenuID menuID, 
                                             ODPlatformMenu menu, 
                                             ODPart* part)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddMenuLast");

    try {

    #ifdef _PLATFORM_WIN32_
	somSelf->AddMenuLastEx(ev, menuID, menu, part, 0, kODNULL);
    #endif // PLATFORM_WIN32

    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)

	if (somSelf->MenuExists(ev, menuID))
	{
	    // do nothing
	}
	else if (_fHelpMenuExists)
	{
	    somSelf->AddMenuBefore(ev, menuID, menu, part, IDMS_HELP);
	}
	else
	{
	// If menu does not already exist, and there is no help menu,
	// create a new platform menu adapter, using the platform menu
	// and add it to the end of the menu list and platform menubar.
	// ------------------------------------------------------------

	#ifdef _PLATFORM_OS2_
	    ODOS2MenuAdapter *newMenuAd = new ODOS2MenuAdapter;
	    newMenuAd->Init(ev, menuID, _fPlatMenuBar);
	    newMenuAd->AddItemsFromPlatMenu(ev, menu);

	    // get the platform menu from the new menu adapter
	    // and insert an popup item for it into the platform menu bar.
	    // --------------------------------------------------------------
	    MENUITEM os2menuItem;

	    os2menuItem.iPosition   = MIT_END;
	    os2menuItem.afStyle     = MIS_SUBMENU | MIS_TEXT;
	    os2menuItem.id	    = menuID;
	    os2menuItem.afAttribute = 0;

	    if (! newMenuAd->IsMenuEnabled(ev))
		os2menuItem.afAttribute |= MIA_DISABLED;

	    newMenuAd->GetPlatMenu(ev, &(os2menuItem.hwndSubMenu));

	    os2menuItem.hItem	    = NULLHANDLE;

	    MRESULT mres = WinSendMsg(_fPlatMenuBar, MM_INSERTITEM,
						     MPFROMP(&os2menuItem),
						     MPFROMP(" "));

	#endif // PLATFORM_OS2

	#ifdef _PLATFORM_AIX_

	    // the platform menu IS the adapter
	    // --------------------------------
	    ODX11MenuAdapter* newMenuAd = (ODX11MenuAdapter*) menu;

	#endif // PLATFORM_AIX

	    // set the menu's part, and add it to _fMenus
	    // ------------------------------------------
	    newMenuAd->SetPart(ev, part);

	    _fMenus->AddLast((ElementType) newMenuAd);

	    if (menuID == IDMS_HELP)
		_fHelpMenuExists = kODTrue;
	}

    #endif // PLATFORM_OS2, PLATFORM_AIX

    #ifdef _PLATFORM_MACINTOSH_

	OrderedCollection* otherMenus = kODNULL; ODVolatile(otherMenus);
	ODPrivMenuInfo* info = kODNULL; ODVolatile(info);

	if (_fOtherMenus == kODNULL)
	{
	    otherMenus = new OrderedCollection;
	    _fOtherMenus = otherMenus;
	}

	info = new ODPrivMenuInfo(menuID, menu,part, 0);
	_fOtherMenus->AddLast((ElementType) info);

	// if this is the current menu bar, we should also tell the Menu Manager
	TempODMenuBar currentMenuBar = _fSession->GetWindowState(ev)->
						AcquireCurrentMenuBar(ev);
	if (ODObjectsAreEqual(ev, currentMenuBar, somSelf))
	{
	    ::InsertMenu(menu, 0);
	    ::InvalMenuBar();
	}

    #endif // PLATFORM_MACINTOSH

    }
    catch (ODException _exception)
    {

    #ifdef _PLATFORM_MACINTOSH_
	ODDeleteObject(otherMenus);
	ODDeleteObject(info);
    #endif // PLATFORM_MACINTOSH

        SetErrorCode(kODNoError);
    }

} /* end AddMenuLast */

//-------------------------------------------------------------------------------------
// AddMenuBefore
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarAddMenuBefore(ODMenuBar *somSelf, 
                                                Environment *ev, 
                                               ODMenuID menuID, 
                                               ODPlatformMenu menu, 
                                               ODPart* part, 
                                               ODMenuID beforeID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddMenuBefore");

    try {

    #ifdef _PLATFORM_WIN32_
	somSelf->AddMenuBeforeEx(ev, menuID, menu, part, beforeID, 0, kODNULL);
    #endif // PLATFORM_WIN32

    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)

	if (somSelf->MenuExists(ev, menuID))
	    THROW(kODErrInvalidParameter);

	// create a new platform menu adapter, using the platform menu
	// -----------------------------------------------------------

	#ifdef _PLATFORM_OS2_

	    ODOS2MenuAdapter *beforeAd = kODNULL;
	    ODUShort beforePos;

	    if (menu == IDMS_HELP)
	    {
		beforePos = MIT_END;
	    }
	    else if (GetOS2MenuAdapter(somSelf, ev, beforeID, &beforeAd))
	    {
		beforePos = SHORT1FROMMR(WinSendMsg(_fPlatMenuBar,
						    MM_ITEMPOSITIONFROMID,
						    MPFROM2SHORT(beforeID,FALSE),
						    0L));
	    }
	    else if ((_fHelpMenuExists) &&
		     (GetOS2MenuAdapter(somSelf, ev, IDMS_HELP, &beforeAd)))
	    {
		beforePos = SHORT1FROMMR(WinSendMsg(_fPlatMenuBar,
						    MM_ITEMPOSITIONFROMID,
						    MPFROM2SHORT(IDMS_HELP,FALSE),
						    0L));
	    }
	    else
	    {
		beforePos = MIT_END;
	    }

	    ODOS2MenuAdapter *newMenuAd = new ODOS2MenuAdapter;
	    newMenuAd->Init(ev, menuID, _fPlatMenuBar);
	    newMenuAd->AddItemsFromPlatMenu(ev, menu);

	    // get the platform menu from the new menu adapter
	    // and insert a popup item for it into the platform menu bar.
	    // ----------------------------------------------------------
	    MENUITEM os2menuItem;

	    os2menuItem.iPosition   = beforePos;
	    os2menuItem.afStyle     = MIS_SUBMENU | MIS_TEXT;
	    os2menuItem.id	    = menuID;
	    os2menuItem.afAttribute = 0;
	    os2menuItem.hItem	    = NULLHANDLE;

	    if (! newMenuAd->IsMenuEnabled(ev))
		os2menuItem.afAttribute |= MIA_DISABLED;

	    newMenuAd->GetPlatMenu(ev, &(os2menuItem.hwndSubMenu));

	    MRESULT mres = WinSendMsg(_fPlatMenuBar, MM_INSERTITEM,
						     MPFROMP(&os2menuItem),
						     MPFROMP(" "));

	#endif // PLATFORM_OS2

	#ifdef _PLATFORM_AIX_

	    // the platform menu IS the adapter
	    // --------------------------------
	    ODX11MenuAdapter* newMenuAd = (ODX11MenuAdapter*) menu;

	    ODX11MenuAdapter *beforeAd = kODNULL;

	    if (menu != IDMS_HELP)
	    {
		if (! GetX11MenuAdapter(somSelf, ev, beforeID, &beforeAd))
		{
		    if (_fHelpMenuExists)
			GetX11MenuAdapter(somSelf, ev, IDMS_HELP, &beforeAd);
		}
	    }

	#endif // PLATFORM_AIX

	// set the menu's part, and add it to _fMenus
	// ------------------------------------------
	newMenuAd->SetPart(ev, part);

	if (menuID == IDMS_HELP)
	{
	    _fMenus->AddLast((ElementType) newMenuAd);
	    _fHelpMenuExists = kODTrue;
	}
	else if (beforeAd)
	{
	    _fMenus->AddBefore((ElementType) beforeAd, (ElementType) newMenuAd);
	}
	else
	{
	    _fMenus->AddLast((ElementType) newMenuAd);
	}

    #endif // PLATFORM_OS2, PLATFORM_AIX

    #ifdef _PLATFORM_MACINTOSH_

	OrderedCollection* otherMenus = kODNULL; ODVolatile(otherMenus);
	ODPrivMenuInfo* info = kODNULL; ODVolatile(info);

	if (_fOtherMenus == kODNULL)
	{
	    otherMenus = new OrderedCollection;
	    _fOtherMenus = otherMenus;
	}

	info = new ODPrivMenuInfo(menuID, menu,part, beforeID);
	_fOtherMenus->AddLast((ElementType) info);

	// if this is the current menu bar, we should also tell the Menu Manager
	TempODMenuBar currentMenuBar = _fSession->GetWindowState(ev)->
							AcquireCurrentMenuBar(ev);
	if (ODObjectsAreEqual(ev, currentMenuBar, somSelf))
	{
	    ::InsertMenu(menu, beforeID);
	    ::InvalMenuBar();
	}
    #endif // PLATFORM_MACINTOSH

    }
    catch (ODException _exception)
    {

    #ifdef _PLATFORM_MACINTOSH_
	ODDeleteObject(otherMenus);
	ODDeleteObject(info);
    #endif // PLATFORM_MACINTOSH

        SetErrorCode(kODNoError);
    }

} /* end AddMenuBefore */

//-------------------------------------------------------------------------------------
// RemoveMenu
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarRemoveMenu(ODMenuBar *somSelf, 
                                             Environment *ev, 
                                            ODMenuID menu)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarRemoveMenu");

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd   = kODNULL;
	ODOS2MenuAdapter* os2parentAd = kODNULL;

	if (GetOS2MenuAdapter(somSelf, ev, menu, &os2menuAd))
	{
	    os2menuAd->GetParentAdapter(ev, &os2parentAd);

	    if (os2parentAd)
	    {
		// if this is a submenu, let the parent
		// menu adapter take care of it...
		// ------------------------------------
		os2parentAd->RemoveMenuItem(ev, menu);
	    }
	    else
	    {
		_fMenus->Remove((ElementType) os2menuAd);

		// remove the platform menu from the platform menu bar.
		// ----------------------------------------------------
		MRESULT mres = WinSendMsg(_fPlatMenuBar, MM_DELETEITEM,
					  MPFROM2SHORT(menu, FALSE), 0L);

		ODDeleteObject(os2menuAd);
	    }
	}
    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd   = kODNULL;
	ODWinMenuAdapter* winParentAd = kODNULL;

	if (GetWinMenuAdapter(somSelf, ev, menu, &winMenuAd))
	{
	    winMenuAd->GetParentAdapter(ev, &winParentAd);

	    if (winParentAd)
	    {
		// if this is a submenu, let the parent
		// menu adapter take care of it...
		// ------------------------------------
		winParentAd->RemoveMenuItem(ev, menu);
	    }
	    else
	    {
		_fMenus->Remove((ElementType) winMenuAd);

		// remove the platform menu from the platform menu bar.
		// ----------------------------------------------------
		ODUShort  pos = winMenuAd->GetPosInParentMenu(ev);
		DWORD     err;
		BOOL      res = RemoveMenu(_fPlatMenuBar, pos, MF_BYPOSITION);

		if (res == TRUE)
		    UpdateWinMenuBar(somSelf, ev);
		else
		    err = GetLastError();

		ODDeleteObject(winMenuAd);
	    }
	}
    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd   = kODNULL;
	ODX11MenuAdapter* x11parentAd = kODNULL;

	if (GetX11MenuAdapter(somSelf, ev, menu, &x11menuAd))
	{
	    x11menuAd->GetParentAdapter(ev, &x11parentAd);

	    if (x11parentAd)
	    {
		// if this is a submenu, let the parent
		// menu adapter take care of it...
		// ------------------------------------
		x11parentAd->RemoveMenuItem(ev, menu);
	    }
	    else
	    {
		_fMenus->Remove((ElementType) x11menuAd);
		ODDeleteObject(x11menuAd);
	    }
	}
    #endif // PLATFORM_AIX

    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)

	if (menu == IDMS_HELP)
	    _fHelpMenuExists = kODFalse;

    #endif // PLATFORM_OS2, PLATFORM_WIN32, PLATFORM_AIX

    #ifdef _PLATFORM_MACINTOSH_

	ODPrivMenuInfo* foundMenu = kODNULL;

	if (_fOtherMenus)
	{
	    OrderedCollectionIterator iter(_fOtherMenus);
	    for (ODPrivMenuInfo* menuInfo = (ODPrivMenuInfo*) iter.First();
		 iter.IsNotComplete();
		 menuInfo = (ODPrivMenuInfo*) iter.Next())
	    {
		if (menuInfo->fMenuID == menu)
		{
		    foundMenu = menuInfo;
		    break;
		}
	    }

	    if (foundMenu)
	    {
		_fOtherMenus->Remove((ElementType) foundMenu);

		// if this is the current menu bar, we should also tell the Menu Manager
		TempODMenuBar currentMenuBar = _fSession->GetWindowState(ev)->
								AcquireCurrentMenuBar(ev);

		if (ODObjectsAreEqual(ev, currentMenuBar, somSelf))
		{
		    ::DeleteMenu(menu);
		    ::InvalMenuBar();
		}
	    }
	    ODDeleteObject(foundMenu);
	}

    #endif // PLATFORM_MACINTOSH

    }
    catch (ODException _exception)
    {
	SetErrorCode(kODNoError);
    }


} /* end RemoveMenu */

//-------------------------------------------------------------------------------------
// GetMenu
//-------------------------------------------------------------------------------------
SOM_Scope ODPlatformMenu  SOMLINK ODMenuBarGetMenu(ODMenuBar *somSelf, 
                                                    Environment *ev, 
                                                   ODMenuID menu)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetMenu");

    ODPlatformMenu platformMenu = kODNULL;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menu, &os2menuAd))
	    os2menuAd->GetPlatMenu(ev, &platformMenu);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menu, &winMenuAd))
	    winMenuAd->GetPlatMenu(ev, &platformMenu);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	// on AIX, the adapter IS the platform menu...
	if (GetX11MenuAdapter(somSelf, ev, menu, &x11menuAd))
	    platformMenu = (ODPlatformMenu) x11menuAd;

    #endif // PLATFORM_AIX

    #ifdef _PLATFORM_MACINTOSH_

	if (_fOtherMenus)
	{
	    OrderedCollectionIterator iter(_fOtherMenus);
	    for (ODPrivMenuInfo* menuInfo = (ODPrivMenuInfo*) iter.First();
		 iter.IsNotComplete();
		 menuInfo = (ODPrivMenuInfo*) iter.Next())
	    {
		if (menuInfo->fMenuID == menu)
		{
		    platformMenu = menuInfo->fMenu;
		    break;
		}
	    }
	}

	// In case it's in the system menus:
	if (platformMenu == kODNULL)
	    platformMenu = GetMenuHandle(menu);

    #endif // PLATFORM_MACINTOSH
	
    }
    catch (ODException _exception)
    {
	platformMenu = kODNULL;
	SetErrorCode(kODNoError);
    }

    return platformMenu;

} /* end GetMenu */

//-------------------------------------------------------------------------------------
// IsValid
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarIsValid(ODMenuBar *somSelf, 
                                               Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarIsValid");

    ODBoolean isValid = kODFalse;

    try {
	ODWindowState* winStat = _fSession->GetWindowState(ev);
	ODMenuBar* baseMenuBar = kODNULL;

	if (winStat)
	    baseMenuBar = winStat->AcquireBaseMenuBar(ev);

	if (baseMenuBar)
	{
	    isValid = _fGeneration == baseMenuBar->GetGeneration(ev);
	    baseMenuBar->Release(ev);
	}
    }
    catch (ODException _exception)
    {
	isValid = kODFalse;
	SetErrorCode(kODNoError);
    }

    return isValid;

} /* end IsValid */


#ifdef _PLATFORM_MACINTOSH_
//=====================================================================================
// METHODS FOR MACINTOSH ONLY
//=====================================================================================

//-------------------------------------------------------------------------------------
// AddSubMenu [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarAddSubMenu(ODMenuBar *somSelf, Environment *ev,
                ODMenuID menuID,
                ODPlatformMenu menu,
                ODPart* part)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddSubMenu");

        OrderedCollection* otherMenus = kODNULL; ODVolatile(otherMenus);
        ODPrivMenuInfo* info = kODNULL; ODVolatile(info);

    SOM_TRY

                if (_fOtherMenus == kODNULL)
                {
                        otherMenus = new OrderedCollection;
                        _fOtherMenus = otherMenus;
                }
                info = new ODPrivMenuInfo(menuID, menu,part, hierMenu);
                _fOtherMenus->AddLast((ElementType) info);

                // if this is the current menu bar, we should also tell the Menu Manager
                TempODMenuBar currentMenuBar = _fSession->GetWindowState(ev)->AcquireCurrentMenuBar(ev);
                if (ODObjectsAreEqual(ev, currentMenuBar, somSelf))
                {
                        ::InsertMenu(menu, hierMenu);
                        ::InvalMenuBar();
                }

        SOM_CATCH_ALL

                ODDeleteObject(otherMenus);
                ODDeleteObject(info);

        SOM_ENDTRY

}

//-------------------------------------------------------------------------------------
// RegisterCommand [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarRegisterCommand(ODMenuBar *somSelf, Environment *ev,
                ODCommandID command,
                ODMenuID menu,
                ODMenuItemID menuItem)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarRegisterCommand");

    CommandTable* table = kODNULL; ODVolatile(table);

    SOM_TRY

                if (_fCommandTable == kODNULL)
                {
                        table = new CommandTable;
                        _fCommandTable = table;
                }
                _fCommandTable->RegisterCommand(command, menu, menuItem);

        SOM_CATCH_ALL

                ODDeleteObject(table);

        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// UnregisterCommand [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarUnregisterCommand(ODMenuBar *somSelf, Environment *ev,
                ODCommandID command)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarUnregisterCommand");

        SOM_TRY

                if (_fCommandTable)
                        _fCommandTable->UnregisterCommand(command);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// UnregisterAll [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarUnregisterAll(ODMenuBar *somSelf, Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarUnregisterAll");

        SOM_TRY

                if (_fCommandTable)
                        _fCommandTable->UnregisterAll();

        SOM_CATCH_ALL
        SOM_ENDTRY
}


//-------------------------------------------------------------------------------------
// IsCommandRegistered [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarIsCommandRegistered(ODMenuBar *somSelf, Environment *ev,
                ODCommandID command)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarIsCommandRegistered");

    ODBoolean isRegistered = kODFalse;

        SOM_TRY

                if (_fCommandTable)
                        isRegistered = _fCommandTable->IsCommandRegistered(command);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return isRegistered;
}

//-------------------------------------------------------------------------------------
// IsCommandSynthetic [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarIsCommandSynthetic(ODMenuBar *somSelf, Environment *ev,
                ODCommandID command)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarIsCommandSynthetic");

        return (command&kSyntheticMask) == kSyntheticMask;
}

//-------------------------------------------------------------------------------------
// GetCommand [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope ODCommandID  SOMLINK ODMenuBarGetCommand(ODMenuBar *somSelf, Environment *ev,
                ODMenuID menu,
                ODMenuItemID menuItem)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetCommand");

        ODCommandID command = kNoCommand;

        SOM_TRY

                if (_fCommandTable)
                        command = _fCommandTable->GetCommand(menu, menuItem);

                if (command == kNoCommand)
                        // No command in table, so synthesize one:
                        command = kSyntheticMask | ((long)menu<<kMenuIDShift) | (menuItem & kMenuItemMask);

        SOM_CATCH_ALL
        SOM_ENDTRY

        return command;
}

//-------------------------------------------------------------------------------------
// GetMenuandItem [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarGetMenuAndItem(ODMenuBar *somSelf, Environment *ev,
                ODCommandID command,
                ODMenuID* menu,
                ODMenuItemID* menuItem)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetMenuAndItem");

        ODBoolean found = kODFalse;

        SOM_TRY

                if (somSelf->IsCommandSynthetic(ev, command))
                {
                        *menu = (ODMenuID) ((command & kMenuIDMask) >>kMenuIDShift);
                        *menuItem = (ODMenuItemID) (command & kMenuItemMask);
                        found = kODTrue;
                }
                else if (_fCommandTable)
                {
                        found = _fCommandTable->GetMenuAndItem(command, *menu, *menuItem);
                }
                if (!found)
                        THROW(kODErrInvalidCommandID);

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// EnableCommand [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarEnableCommand(ODMenuBar *somSelf, Environment *ev,
                ODCommandID cmdNumber,
                ODBoolean enable)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarEnableCommand");

        SOM_TRY

                ODMenuID menuID;
                ODMenuItemID item;
                somSelf->GetMenuAndItem( ev, cmdNumber, &menuID, &item ) ; // THROWS
                ODPlatformMenu macMenu = somSelf->GetMenu( ev, menuID ) ;
                if (macMenu)
                {
                        if (enable)
                                EnableItem(macMenu, item) ;
                        else
                                DisableItem(macMenu, item) ;
                }

        SOM_CATCH_ALL

                SetErrorCode(kODNoError);

        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// CheckCommand [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarCheckCommand(ODMenuBar *somSelf, Environment *ev,
                ODCommandID cmdNumber,
                ODBoolean check)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarCheckCommand");

    SOM_TRY

                ODMenuID menuID;
                ODMenuItemID item;
                somSelf->GetMenuAndItem( ev, cmdNumber, &menuID, &item ) ;
                ODPlatformMenu macMenu = somSelf->GetMenu( ev, menuID ) ;
                if (macMenu)
                        CheckItem(macMenu, item, check);

        SOM_CATCH_ALL

                SetErrorCode(kODNoError);

        SOM_ENDTRY

}

//-------------------------------------------------------------------------------------
// EnableAndCheckCommand [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarEnableAndCheckCommand(ODMenuBar *somSelf, Environment *ev,
                ODCommandID cmdNumber,
                ODBoolean enable,
                ODBoolean check)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarEnableAndCheckCommand");

    SOM_TRY

                ODMenuID menuID;
                ODMenuItemID item;
                somSelf->GetMenuAndItem( ev, cmdNumber, &menuID, &item ) ;
                ODPlatformMenu macMenu = somSelf->GetMenu( ev, menuID ) ;
                if (macMenu)
                {
                if ( enable )
                        EnableItem(macMenu, item) ;
                else
                        DisableItem(macMenu, item) ;
                CheckItem(macMenu, item, check);
                }

        SOM_CATCH_ALL

                SetErrorCode(kODNoError);

        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// SetItemString [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarSetItemString(ODMenuBar *somSelf, Environment *ev,
                ODCommandID cmdNumber,
                ODIText* itemString)
{
        const ODSShort  kUseSpecificScript = 0x1c;

    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarSetItemString");

        SOM_TRY

                ODMenuID menuID;
                ODMenuItemID item;
                somSelf->GetMenuAndItem( ev, cmdNumber, &menuID, &item ) ;
                ODPlatformMenu macMenu = somSelf->GetMenu( ev, menuID ) ;
                if( macMenu != kODNULL )
                {
                        StringPtr s = GetPStringFromIText(itemString);
                        SetMenuItemText(macMenu, item, s);
                        ODDisposePtr(s);

                        short scriptID = GetITextScriptCode(itemString);
                        ODSLong savedSMFontForceFlag = GetScriptManagerVariable(smFontForce);
                        SetScriptManagerVariable(smFontForce, false);

                        if (scriptID != FontToScript(GetAppFont()))
                        {
                                SetItemCmd(macMenu, item, kUseSpecificScript);
                                SetItemIcon(macMenu, item, scriptID);
                        }

                        SetScriptManagerVariable(smFontForce, savedSMFontForceFlag);
                }

        SOM_CATCH_ALL

                SetErrorCode(kODNoError);

        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// GetItemString [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarGetItemString(ODMenuBar *somSelf, Environment *ev,
                ODCommandID cmdNumber,
                ODIText* itemString)
{
        const ODSShort  kUseSpecificScript = 0x1c;

    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetItemString");

    SOM_TRY

                ODMenuID menuID;
                ODMenuItemID item;
                somSelf->GetMenuAndItem( ev, cmdNumber, &menuID, &item ) ;
                ODPlatformMenu macMenu = somSelf->GetMenu( ev, menuID ) ;
                if (macMenu)
                {
                        if (itemString)
                        {
                                itemString->format = kODTraditionalMacText;
                                itemString->text._buffer = kODNULL;
                        }

                        Str255 s;
                        GetMenuItemText(macMenu, item, s);
                        SetITextString(itemString, s);

                        // Is this correct?
                        short cmd;
                        short scriptID = smSystemScript;
                        ODSLong savedSMFontForceFlag = GetScriptManagerVariable(smFontForce);
                        SetScriptManagerVariable(smFontForce, false);
                        GetItemCmd (macMenu, item, &cmd);
                        if (cmd == kUseSpecificScript)
                        {
                                GetItemIcon (macMenu, item, &scriptID);
                        }
                        SetITextScriptCode(itemString, scriptID);
                        SetITextLangCode(itemString, langEnglish);
                        SetScriptManagerVariable(smFontForce, savedSMFontForceFlag);
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// DisableAll [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarDisableAll(ODMenuBar *somSelf, Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarDisableAll");

        SOM_TRY

                if (_fOtherMenus)
                {
                        ODPlatformMenu platformMenu = kODNULL;
                        OrderedCollectionIterator iter(_fOtherMenus);
                        for (ODPrivMenuInfo* menuInfo = (ODPrivMenuInfo*) iter.First();
                                iter.IsNotComplete();
                                menuInfo = (ODPrivMenuInfo*) iter.Next())
                        {
                                platformMenu = menuInfo->fMenu;
                                DisableItem(platformMenu,0);
                        }
                }

        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// EnableAll [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarEnableAll(ODMenuBar *somSelf, Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarEnableAll");

        SOM_TRY

                if (_fOtherMenus)
                {
                        ODPlatformMenu platformMenu = kODNULL;
                        OrderedCollectionIterator iter(_fOtherMenus);
                        for (ODPrivMenuInfo* menuInfo = (ODPrivMenuInfo*) iter.First();
                                iter.IsNotComplete();
                                menuInfo = (ODPrivMenuInfo*) iter.Next())
                        {
                                platformMenu = menuInfo->fMenu;
                                EnableItem(platformMenu,0);
                        }
                }
        SOM_CATCH_ALL
        SOM_ENDTRY
}

                                EnableItem(platformMenu,0);
                        }
                }
        SOM_CATCH_ALL
        SOM_ENDTRY
}

//-------------------------------------------------------------------------------------
// SetCommandTable [PRIVATE] [Macintosh only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarSetCommandTable(ODMenuBar *somSelf, Environment *ev,
                CommandTable* table)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarSetCommandTable");

        _fCommandTable = table;
}

#endif // PLATFORM_MACINTOSH


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
//=====================================================================================
// METHODS FOR OS/2, WINDOWS, AND AIX ONLY
//=====================================================================================

//-------------------------------------------------------------------------------------
// GetMenuText [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODUShort  SOMLINK ODMenuBarGetMenuText(ODMenuBar *somSelf, 
                                                  Environment *ev, 
                                                 ODMenuID menuID, 
                                                 string* menuString)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetMenuText");

    ODUShort sLength = 0;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    sLength = os2menuAd->GetMenuText(ev, menuString);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    sLength = winMenuAd->GetMenuText(ev, menuString);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    sLength = x11menuAd->GetMenuText(ev, menuString);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	sLength = 0;
	SetErrorCode(kODNoError);
    }

    return sLength;

} /* end GetMenuText */

//-------------------------------------------------------------------------------------
// SetMenuText [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarSetMenuText(ODMenuBar *somSelf, 
                                                   Environment *ev, 
                                                  ODMenuID menuID, 
                                                  string menuString)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarSetMenuText");

    ODBoolean bResult = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->SetMenuText(ev, menuString);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    bResult = winMenuAd->SetMenuText(ev, menuString);

	if (bResult == kODTrue)
	    UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->SetMenuText(ev, menuString);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end SetMenuText */

//-------------------------------------------------------------------------------------
// GetMenuItemText [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODUShort  SOMLINK ODMenuBarGetMenuItemText(ODMenuBar *somSelf, 
                                                      Environment *ev, 
                                                     ODMenuID menuID, 
                                                     ODMenuItemID menuItemID, 
                                                     string* menuItemString)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetMenuItemText");

    ODUShort sLength = 0;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
	return (somSelf->GetMenuText(ev, (ODMenuID) menuItemID, menuItemString));

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    sLength = os2menuAd->GetMenuItemText(ev, menuItemID, menuItemString);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    sLength = winMenuAd->GetMenuItemText(ev, menuItemID, menuItemString);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    sLength = x11menuAd->GetMenuItemText(ev, menuItemID, menuItemString);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	sLength = 0;
	SetErrorCode(kODNoError);
    }

    return sLength;

} /* end GetMenuItemText */

//-------------------------------------------------------------------------------------
// SetMenuItemText [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarSetMenuItemText(ODMenuBar *somSelf, 
                                                       Environment *ev, 
                                                      ODMenuID menuID, 
                                                      ODMenuItemID menuItemID, 
                                                      string menuItemString)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarSetMenuItemText");

    ODBoolean bResult = kODFalse;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
	return (somSelf->SetMenuText(ev, (ODMenuID) menuItemID, menuItemString));

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->SetMenuItemText(ev, menuItemID, menuItemString);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    bResult = winMenuAd->SetMenuItemText(ev, menuItemID, menuItemString);

	if (bResult == kODTrue)
	    UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->SetMenuItemText(ev, menuItemID, menuItemString);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end SetMenuItemText */

//-------------------------------------------------------------------------------------
// SetHideMenuItem [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarSetHideMenuItem(ODMenuBar *somSelf, 
                                                       Environment *ev, 
                                                      ODMenuID menuID, 
                                                      ODMenuItemID menuItemID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarSetHideMenuItem");

    ODBoolean bResult = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->SetHideMenuItem(ev, menuItemID);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    bResult = winMenuAd->SetHideMenuItem(ev, menuItemID);

	if (bResult == kODTrue)
	    UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->SetHideMenuItem(ev, menuItemID);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end SetHideMenuItem */

//-------------------------------------------------------------------------------------
// SetShowMenuItem [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarSetShowMenuItem(ODMenuBar *somSelf, 
                                                       Environment *ev, 
                                                      ODMenuID menuID, 
                                                      ODMenuItemID menuItemID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarSetShowMenuItem");

    ODBoolean bResult = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->SetShowMenuItem(ev, menuItemID);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    bResult = winMenuAd->SetShowMenuItem(ev, menuItemID);

	if (bResult == kODTrue)
	    UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->SetShowMenuItem(ev, menuItemID);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end SetShowMenuItem */

//-------------------------------------------------------------------------------------
// CheckMenuItem [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarCheckMenuItem(ODMenuBar *somSelf, 
                                                     Environment *ev, 
                                                    ODMenuID menuID, 
                                                    ODMenuItemID menuItemID, 
                                                    ODBoolean check)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarCheckMenuItem");

    ODBoolean bResult = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->CheckMenuItem(ev, menuItemID, check);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    bResult = winMenuAd->CheckMenuItem(ev, menuItemID, check);

	if (bResult == kODTrue)
	    UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->CheckMenuItem(ev, menuItemID, check);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end CheckMenuItem */

//-------------------------------------------------------------------------------------
// EnableMenu [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarEnableMenu(ODMenuBar *somSelf, 
                                                  Environment *ev, 
                                                 ODMenuID menuID, 
                                                 ODBoolean enable)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarEnableMenu");

    ODBoolean bResult = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	{
	    if (enable)
		os2menuAd->EnableMenu(ev);
	    else
		os2menuAd->DisableMenu(ev);
	}
    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	{
	    if (enable)
		winMenuAd->EnableMenu(ev);
	    else
		winMenuAd->DisableMenu(ev);

	    UpdateWinMenuBar(somSelf, ev);
	}
    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	{
	    if (enable)
		x11menuAd->EnableMenu(ev);
	    else
		x11menuAd->DisableMenu(ev);
	}
    #endif // PLATFORM_AIX

	bResult = kODTrue;

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end EnableMenu */

//-------------------------------------------------------------------------------------
// EnableMenuItem [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarEnableMenuItem(ODMenuBar *somSelf, 
                                                      Environment *ev, 
                                                     ODMenuID menuID, 
                                                     ODMenuItemID menuItemID, 
                                                     ODBoolean enable)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarEnableMenuItem");

    ODBoolean bResult = kODFalse;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
	return (somSelf->EnableMenu(ev, (ODMenuID) menuItemID, enable));

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->EnableMenuItem(ev, menuItemID, enable);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    bResult = winMenuAd->EnableMenuItem(ev, menuItemID, enable);

	if (bResult == kODTrue)
	    UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->EnableMenuItem(ev, menuItemID, enable);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end EnableMenuItem */

//-------------------------------------------------------------------------------------
// AddMenuItemBefore [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarAddMenuItemBefore(ODMenuBar *somSelf, 
                                                         Environment *ev, 
                                                        ODMenuID menuID, 
                                                        ODMenuItemID itemID, 
                                                        ODPlatformMenuItem* menuItem, 
                                                        ODMenuItemID beforeID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddMenuItemBefore");

    ODBoolean bResult = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->AddMenuItemBefore(ev, itemID, menuItem, beforeID);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    bResult = winMenuAd->AddMenuItemBefore(ev, itemID, menuItem, beforeID);

	if (bResult == kODTrue)
	    UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->AddMenuItemBefore(ev, itemID, menuItem, beforeID);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end AddMenuItemBefore */

//-------------------------------------------------------------------------------------
// AddMenuItemLast [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarAddMenuItemLast(ODMenuBar *somSelf, 
                                                       Environment *ev, 
                                                      ODMenuID menuID, 
                                                      ODMenuItemID itemID, 
                                                      ODPlatformMenuItem* menuItem)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddMenuItemLast");

    ODBoolean bResult = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->AddMenuItemLast(ev, itemID, menuItem);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    bResult = winMenuAd->AddMenuItemLast(ev, itemID, menuItem);

	if (bResult == kODTrue)
	    UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->AddMenuItemLast(ev, itemID, menuItem);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end AddMenuItemLast */

//-------------------------------------------------------------------------------------
// AddSubMenuBefore [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarAddSubMenuBefore(ODMenuBar *somSelf, 
							Environment *ev, 
							ODMenuID menuID, 
							ODMenuItemID subMenuItemID, 
							ODPlatformMenu newSubMenu,
							ODMenuItemID beforeID) 
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddSubMenuBefore");

    ODBoolean bResult = kODFalse;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
    {
	somSelf->AddMenuBefore(ev, (ODMenuID) subMenuItemID, newSubMenu,
					      kODNULL, beforeID);
	return kODTrue;
    }

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->AddSubMenuBefore(ev, subMenuItemID,
						      newSubMenu, beforeID);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	bResult = somSelf->AddSubMenuBeforeEx(ev, menuID, subMenuItemID,
					      newSubMenu, beforeID, 0, kODNULL);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->AddSubMenuBefore(ev, subMenuItemID,
						      newSubMenu, beforeID);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end AddSubMenuBefore */

//-------------------------------------------------------------------------------------
// AddSubMenuLast [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarAddSubMenuLast(ODMenuBar *somSelf, 
						      Environment *ev, 
						     ODMenuID menuID, 
						     ODMenuItemID subMenuItemID, 
						     ODPlatformMenu newSubMenu)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddSubMenuLast");

    ODBoolean bResult = kODFalse;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
    {
	somSelf->AddMenuLast(ev, (ODMenuID) subMenuItemID, newSubMenu, kODNULL);

	return kODTrue;
    }

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->AddSubMenuLast(ev, subMenuItemID, newSubMenu);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	bResult = somSelf->AddSubMenuLastEx(ev, menuID, subMenuItemID,
						newSubMenu, 0, kODNULL);
    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->AddSubMenuLast(ev, subMenuItemID, newSubMenu);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end AddSubMenuLast */

//-------------------------------------------------------------------------------------
// InsertSubmenu [OBSOLETE] [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarInsertSubmenu(ODMenuBar *somSelf, 
						      Environment *ev, 
						     ODMenuID menuID, 
						     ODMenuItemID subMenuID, 
						     ODPlatformMenu newSubMenu)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarInsertSubmenu");

    return (somSelf->AddSubMenuLast(ev, menuID, subMenuID, newSubMenu));

} /* end InsertSubmenu */

//-------------------------------------------------------------------------------------
// MenuExists [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarMenuExists(ODMenuBar *somSelf, 
						  Environment *ev, 
						 ODMenuID menuID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarMenuExists");

    ODBoolean exists = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;
	exists = (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd));
    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;
	exists = (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd));
    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;
	exists = (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd));
    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	exists = kODFalse;
        SetErrorCode(kODNoError);
    }

    return exists;

} /* end MenuExists */

//-------------------------------------------------------------------------------------
// IsMenuEnabled [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarIsMenuEnabled(ODMenuBar *somSelf, 
						     Environment *ev, 
						    ODMenuID menuID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarMenuIsMenuEnabled");

    ODBoolean enabled = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    enabled = os2menuAd->IsMenuEnabled(ev);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    enabled = winMenuAd->IsMenuEnabled(ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    enabled = x11menuAd->IsMenuEnabled(ev);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	enabled = kODFalse;
        SetErrorCode(kODNoError);
    }

    return enabled;

} /* end IsMenuEnabled */


//-------------------------------------------------------------------------------------
// MenuItemExists [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarMenuItemExists(ODMenuBar *somSelf, 
						      Environment *ev, 
						     ODMenuID menuID, 
						     ODMenuItemID menuItemID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarMenuItemExists");

    ODBoolean exists = kODFalse;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
	return (somSelf->MenuExists(ev, (ODMenuID) menuItemID));

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    exists = os2menuAd->ItemExists(ev, menuItemID);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    exists = winMenuAd->ItemExists(ev, menuItemID);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    exists = x11menuAd->ItemExists(ev, menuItemID);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	exists = kODFalse;
	SetErrorCode(kODNoError);
    }

    return exists;

} /* end MenuItemExists */

//-------------------------------------------------------------------------------------
// Exists [OBSOLETE] [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarExists(ODMenuBar *somSelf, 
                                              Environment *ev, 
                                             ODMenuID menuID, 
                                             ODMenuItemID menuItemID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarExists");

    return (somSelf->MenuItemExists(ev, menuID, menuItemID));
    
} /* end Exists */

//-------------------------------------------------------------------------------------
// IsItemEnabled [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarIsItemEnabled(ODMenuBar *somSelf, 
                                                     Environment *ev, 
                                                    ODMenuID menuID, 
                                                    ODMenuItemID menuItemID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarIsItemEnabled");

    ODBoolean enabled = kODFalse;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
	return (somSelf->IsMenuEnabled(ev, (ODMenuID) menuItemID));

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    enabled = os2menuAd->IsItemEnabled(ev, menuItemID);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    enabled = winMenuAd->IsItemEnabled(ev, menuItemID);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    enabled = x11menuAd->IsItemEnabled(ev, menuItemID);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	enabled = kODFalse;
	SetErrorCode(kODNoError);
    }

    return enabled;

} /* end IsItemEnabled */

//-------------------------------------------------------------------------------------
// IsItemChecked [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarIsItemChecked(ODMenuBar *somSelf, 
                                                     Environment *ev, 
                                                    ODMenuID menuID, 
                                                    ODMenuItemID menuItemID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarIsItemChecked");

    ODBoolean checked = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    checked = os2menuAd->IsItemChecked(ev, menuItemID);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    checked = winMenuAd->IsItemChecked(ev, menuItemID);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    checked = x11menuAd->IsItemChecked(ev, menuItemID);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	checked = kODFalse;
	SetErrorCode(kODNoError);
    }

    return checked;

} /* end IsItemChecked */

//-------------------------------------------------------------------------------------
// IsItemHidden [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarIsItemHidden(ODMenuBar *somSelf, 
                                                    Environment *ev, 
                                                   ODMenuID menuID, 
                                                   ODMenuItemID menuItemID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarIsItemHidden");

    ODBoolean hidden = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    hidden = os2menuAd->IsItemHidden(ev, menuItemID);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    hidden = winMenuAd->IsItemHidden(ev, menuItemID);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    hidden = x11menuAd->IsItemHidden(ev, menuItemID);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	hidden = kODFalse;
	SetErrorCode(kODNoError);
    }

    return hidden;

} /* end IsItemHidden */

//-------------------------------------------------------------------------------------
// RemoveMenuItem [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarRemoveMenuItem(ODMenuBar *somSelf, 
                                                      Environment *ev, 
                                                     ODMenuID menuID, 
                                                     ODMenuItemID menuItemID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarRemoveMenuItem");

    ODBoolean bResult = kODFalse;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
    {
	somSelf->RemoveMenu(ev, (ODMenuID) menuItemID);
	return kODTrue;
    }

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	    bResult = os2menuAd->RemoveMenuItem(ev, menuItemID);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	    bResult = winMenuAd->RemoveMenuItem(ev, menuItemID);

	if (bResult == kODTrue)
	    UpdateWinMenuBar(somSelf, ev);

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	    bResult = x11menuAd->RemoveMenuItem(ev, menuItemID);

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end RemoveMenuItem */

//-------------------------------------------------------------------------------------
// GetMenuItem [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarGetMenuItem(ODMenuBar *somSelf, 
                                                   Environment *ev, 
                                                  ODMenuID menuID, 
                                                  ODMenuItemID menuItemID, 
                                                  ODPlatformMenuItem* menuItem)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetMenuItem");

    ODBoolean bResult = kODFalse;

    try {

    #ifdef _PLATFORM_OS2_
	ODOS2MenuAdapter* os2menuAd;

	if (GetOS2MenuAdapter(somSelf, ev, menuID, &os2menuAd))
	{
	    ODOS2MenuItemAdapter* itemAd;

	    if (os2menuAd->GetMenuItemAdapter(ev, menuItemID, &itemAd))
	    {
		ODUShort pos;
		bResult = os2menuAd->GetPlatMenuItem(ev, itemAd, menuItem);
		os2menuAd->GetMenuItemPosition(ev, menuItemID, &pos);
		menuItem->iPosition = pos;
	    }
	}

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	{
	    ODWinMenuItemAdapter* itemAd;

	    if (winMenuAd->GetMenuItemAdapter(ev, menuItemID, &itemAd))
	    {
		ODUShort pos;
		bResult = winMenuAd->GetPlatMenuItem(ev, itemAd, menuItem);
		winMenuAd->GetMenuItemPosition(ev, menuItemID, &pos);
		menuItem->iPosition = pos;
	    }
	}

    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	ODX11MenuAdapter* x11menuAd;

	if (GetX11MenuAdapter(somSelf, ev, menuID, &x11menuAd))
	{
	    bResult = x11menuAd->GetMenuItem(ev, menuItemID, menuItem);
	}

    #endif // PLATFORM_AIX

    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end GetMenuItem */

//-------------------------------------------------------------------------------------
// AddToAccelTable [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean  SOMLINK ODMenuBarAddToAccelTable(ODMenuBar *somSelf, 
                                                       Environment *ev, 
                                                      ODULong ulNumberofAccels, 
                                                      ODACCEL* NewAccels)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddToAccelTable");

    ODBoolean bResult = kODFalse;

    // if greater than 20, return false to caller
    // ----------------------------------------------------------------
    if (ulNumberofAccels > 20)
	return bResult;

    if (ulNumberofAccels > 0)
    {

    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)

	// store passed-in data in our instance variables; set changed flag
	// ----------------------------------------------------------------
	_fNumberofAccels = ulNumberofAccels;

	memcpy(&_fNewAccels, NewAccels, sizeof(ODACCEL));

	_fAccelsToAdd = kODTrue;
	bResult = kODTrue;
	somSelf->ChangeAccelTable(ev);

    #endif // PLATFORM_OS2, PLATFORM_WIN32

    }

    return bResult;

} /* end AddToAccelTable */

//-------------------------------------------------------------------------------------
// RestoreAccelTable [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarRestoreAccelTable(ODMenuBar *somSelf, 
                                                    Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarRestoreAccelTable");


    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)

	HWND hwndFrame = NULLHANDLE;

	// this will create an accel table with defaults and active part accels
	// --------------------------------------------------------------------
	somSelf->ChangeAccelTable(ev);

	#ifdef _PLATFORM_OS2_

	    ODWindow* aWindow = _fSession->GetWindowState(ev)->
						AcquireActiveWindow(ev);
	    hwndFrame =  aWindow->GetPlatformWindow(ev);
	    aWindow->Release(ev);

	    // set this accel table as the default
	    // -----------------------------------
	    WinSetAccelTable(WinQueryAnchorBlock(HWND_DESKTOP),
				_fDefaultAccelTable, hwndFrame);

	    MRESULT mres = WinSendMsg(hwndFrame, WM_UPDATEFRAME,
						 MPFROMLONG(FCF_ACCELTABLE), NULL);

	#endif // PLATFORM_OS2

	#ifdef _PLATFORM_WIN32_

	    // Destroy old _fNewAccelTable and assign this new one to _fNewAccelTable
	    // ----------------------------------------------------------------------
	    if (_fNewAccelTable)
	    {
		if (_fNewAccelTable != _fDefaultAccelTable)
		{
		    DestroyAcceleratorTable(_fNewAccelTable);
		    _fNewAccelTable = _fDefaultAccelTable;
		}
	    }

	#endif //PLATFORM_WIN32

    #endif // PLATFORM_OS2, PLATFORM_WIN32

} /* end RestoreAccelTable */


//-------------------------------------------------------------------------------------
// ChangeAccelTable [PRIVATE] [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarChangeAccelTable(ODMenuBar *somSelf, 
                                                   Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarChangeAccelTable");

    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)

	ODULong ulSize = 0;
	ODULong ulCopied = 0;
	ODULong ulCols = 0;
	ODULong newCols = 0;
	ODACCELTABLE *TempTable;
	ODACCELTABLE *NewTable;
	ODULong i = 0;
	ODULong j = 0;

	if (_fNewAccelTable != NULLHANDLE)
	{

	#ifdef _PLATFORM_OS2_
	    WinDestroyAccelTable(_fNewAccelTable);
	#endif // PLATFORM_OS2

	#ifdef _PLATFORM_WIN32_
	    DestroyAcceleratorTable(_fNewAccelTable);
	#endif //PLATFORM_WIN32

	    _fNewAccelTable = NULLHANDLE;
	}

	// Whether the accelerator table is changed or not, we want to copy
	// the default table ; accelerator tables are destroyed when removed
	// and we DON'T want to destroy our default table ... we'll copy it
	// and let the copy get destroyed each time.
	// We only destroy the default table during somUninit()
	// -----------------------------------------------------------------

	// copy our default table to a temporary table

	#ifdef _PLATFORM_OS2_
	    ulSize = WinCopyAccelTable(_fDefaultAccelTable, NULL, 0);
	#endif // PLATFORM_OS2

	#ifdef _PLATFORM_WIN32_
	    ulCols = CopyAcceleratorTable(_fDefaultAccelTable, NULL, 0);
	    ulSize = ulCols * sizeof(ACCEL);
	#endif //PLATFORM_WIN32

	TempTable = (ODACCELTABLE*)malloc(sizeof(ODACCELTABLE) + ulSize - sizeof(ACCEL));

	#ifdef _PLATFORM_OS2_
	    ulCopied = WinCopyAccelTable(_fDefaultAccelTable, (PACCELTABLE)TempTable, ulSize);
	    ulCols = TempTable->cAccel;
	#endif // PLATFORM_OS2

	#ifdef _PLATFORM_WIN32_
	    TempTable->cAccel = ulCols;
	    ulCopied = CopyAcceleratorTable(_fDefaultAccelTable, TempTable->aaccel,
								 TempTable->cAccel);
	#endif //PLATFORM_WIN32

	// Allocate memory for a new table.  The size is either the
	// default table size, or the default table plus additional accels
	// ---------------------------------------------------------------
	newCols = ulCols + _fNumberofAccels;
	_fNumberofAccels = 0;
	NewTable = (ODACCELTABLE*)malloc(sizeof(ODACCELTABLE) + ((newCols-1)* sizeof(ACCEL)));
	NewTable->cAccel = newCols;
	NewTable->codepage = TempTable->codepage;

	// Copy our default table to the new table
	// ---------------------------------------
	for (i=0;i<ulCols ;i++ )
	{

	#ifdef _PLATFORM_OS2_
	    NewTable->aaccel[i].fs = TempTable->aaccel[i].fs;
	#endif // PLATFORM_OS2
	
	#ifdef _PLATFORM_WIN32_
	    NewTable->aaccel[i].fVirt = TempTable->aaccel[i].fVirt;
	#endif // PLATFORM_OS2

	    NewTable->aaccel[i].key = TempTable->aaccel[i].key;
	    NewTable->aaccel[i].cmd = TempTable->aaccel[i].cmd;
	}

	// If there are accels to add, copy them to the end of the table
	// -------------------------------------------------------------
	if (_fAccelsToAdd)
	{
	    for (i=0, j=ulCols;j<newCols ;i++,j++ ) /* 136756: To avoid break in text part */
	    {
	    #ifdef _PLATFORM_OS2_
		NewTable->aaccel[j].fs  = _fNewAccels.aAccel[i].fs;
	    #endif // PLATFORM_OS2

	    #ifdef _PLATFORM_WIN32_
		NewTable->aaccel[j].fVirt  = _fNewAccels.aAccel[i].fVirt;
	    #endif // PLATFORM_OS2

		NewTable->aaccel[j].key = _fNewAccels.aAccel[i].key;
		NewTable->aaccel[j].cmd = _fNewAccels.aAccel[i].cmd;
	    }
	    _fAccelsToAdd = kODFalse;
	}

	// Our table is fully constructed, so now we create a new accel table.
	// It is either a duplicate of the default table or a composed table
	// ------------------------------------------------------------------

	#ifdef _PLATFORM_OS2_
	    _fNewAccelTable = WinCreateAccelTable(WinQueryAnchorBlock(HWND_DESKTOP),
							(PACCELTABLE)NewTable);
	#endif // PLATFORM_OS2

	#ifdef _PLATFORM_WIN32_
	    _fNewAccelTable = CreateAcceleratorTable(NewTable->aaccel, newCols);
	#endif //PLATFORM_WIN32

	// Free up the memory we allocated
	// -------------------------------
	free(NewTable);
	free(TempTable);

    #endif // PLATFORM_OS2, PLATFORM_WIN32

} /* end ChangeAccelTable */

//-------------------------------------------------------------------------------------
// SetAccelTableToWindow [PRIVATE] [OS/2, WIN32, AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarSetAccelTableToWindow(ODMenuBar *somSelf,
                                                        Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarSetAccelTableToWindow");

    // get the accelerator table ready
    // -------------------------------
    somSelf->ChangeAccelTable(ev);

    #ifdef _PLATFORM_OS2_

	ODWindow* aWindow = _fSession->GetWindowState(ev)->AcquireActiveWindow(ev);
	ODPlatformWindow hwndFrame = aWindow->GetPlatformWindow(ev);
	aWindow->Release(ev);

	// now set our part's accel table
	// ------------------------------
	WinSetAccelTable(WinQueryAnchorBlock(HWND_DESKTOP), _fNewAccelTable, hwndFrame);

	// Once the accelerator table is set, we can clear the handle.
	// It will be destroyed by PM, or by another part setting the table.
	// -----------------------------------------------------------------
	_fNewAccelTable = NULLHANDLE;

	MRESULT mres = WinSendMsg(hwndFrame, WM_UPDATEFRAME,
					     MPFROMLONG(FCF_ACCELTABLE), NULL);

    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	// Nothing to do ...
	// -----------------
    #endif //PLATFORM_WIN32

} /* end SetAccelTableToWindow */

//-------------------------------------------------------------------------------------
// GetAccelTable [PRIVATE] [OS/2, WIN32, AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODPlatformAccelTable SOMLINK ODMenuBarGetAccelTable(ODMenuBar *somSelf,
								Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetAccelTable");

    return _fNewAccelTable;

} /* end GetAccelTable */

//-------------------------------------------------------------------------------------
// SetAccelTable [PRIVATE] [OS/2, WIN32, AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope void SOMLINK ODMenuBarSetAccelTable(ODMenuBar *somSelf,
						Environment *ev,
						ODPlatformAccelTable accelTable)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarSetAccelTable");

    _fDefaultAccelTable = accelTable;

} /* end SetAccelTable */

//-------------------------------------------------------------------------------------
// GetSystemMenus [PRIVATE] [OS/2, WIN32, AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODPlatformMenuBar SOMLINK ODMenuBarGetSystemMenus(ODMenuBar *somSelf,
							     Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetSystemMenus");

    #ifdef _PLATFORM_AIX_
	return  somSelf;
    #else
	return  _fPlatMenuBar;
    #endif

} /* GetSystemMenus */

#endif // PLATFORM_OS2, PLATFORM_WIN32, PLATFORM_AIX

#ifdef _PLATFORM_WIN32_
//=====================================================================================
// METHODS FOR WINDOWS ONLY
//=====================================================================================
//-------------------------------------------------------------------------------------
// AddMenuBeforeEx [WIN32 only]
//-------------------------------------------------------------------------------------
SOM_Scope void SOMLINK ODMenuBarAddMenuBeforeEx(ODMenuBar *somSelf,
						 Environment *ev,
						 ODMenuID menuID,
						 ODPlatformMenu menu,
						 ODPart* part,
						 ODMenuID beforeID,
						 ODUShort subMenuIDcount,
						 ODMenuIDInfo *subMenuIDinfo)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddMenuBeforeEx");

    try {

	if (somSelf->MenuExists(ev, menuID))
	    THROW(kODErrInvalidParameter);

	// create a new platform menu adapter, using the platform menu
	// -----------------------------------------------------------

	ODWinMenuAdapter *beforeAd  = kODNULL;
	ODUShort          beforePos = 0xffffffff;

	if (menuID != IDMS_HELP)
	{
	    if (GetWinMenuAdapter(somSelf, ev, beforeID, &beforeAd))
	    {
		beforePos = beforeAd->GetPosInParentMenu(ev);
	    }
	    else if ((_fHelpMenuExists) &&
		     (GetWinMenuAdapter(somSelf, ev, IDMS_HELP, &beforeAd)))
	    {
		beforePos = beforeAd->GetPosInParentMenu(ev);
	    }
	}

	ODWinMenuAdapter *newMenuAd = new ODWinMenuAdapter;
	newMenuAd->Init(ev, menuID, _fPlatMenuBar);
	newMenuAd->AddItemsFromPlatMenuEx(ev, menu, subMenuIDcount,
						    subMenuIDinfo);

	// get the platform menu from the new menu adapter
	// and insert a popup item for it into the platform menu bar.
	// ----------------------------------------------------------
	ODPlatformMenu subMenu = NULL;
	newMenuAd->GetPlatMenu(ev, &subMenu);

	BOOL	  res;
	DWORD     err;

	res = InsertMenu(_fPlatMenuBar, beforePos,
			 (MF_BYPOSITION | MF_POPUP),
			 (UINT)subMenu, NULL);

	if (res == TRUE)
	    UpdateWinMenuBar(somSelf, ev);
	else
	    err = GetLastError();

	// set the menu's part, and add it to _fMenus
	// ------------------------------------------
	newMenuAd->SetPart(ev, part);

	if (menuID == IDMS_HELP)
	{
	    _fMenus->AddLast((ElementType) newMenuAd);
	    _fHelpMenuExists = kODTrue;
	}
	else if (beforeAd)
	{
	    _fMenus->AddBefore((ElementType) beforeAd, (ElementType) newMenuAd);
	}
	else
	{
	    _fMenus->AddLast((ElementType) newMenuAd);
	}

    }
    catch (ODException _exception)
    {
        SetErrorCode(kODNoError);
    }

} /* end AddMenuBeforeEx */

//-------------------------------------------------------------------------------------
// AddMenuLastEx [WIN32 only]
//-------------------------------------------------------------------------------------
SOM_Scope void SOMLINK ODMenuBarAddMenuLastEx(ODMenuBar *somSelf,
						Environment *ev,
						ODMenuID menuID,
						ODPlatformMenu menu,
						ODPart* part,
						ODUShort subMenuIDcount,
						ODMenuIDInfo *subMenuIDinfo)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddMenuLastEx");

    try {

	if (somSelf->MenuExists(ev, menuID))
	{
	    // do nothing
	}
	else if (_fHelpMenuExists)
	{
	    somSelf->AddMenuBeforeEx(ev, menuID, menu, part, IDMS_HELP,
					 subMenuIDcount, subMenuIDinfo);
	}
	else
	{
	// If menu does not already exist, and there is no help menu,
	// create a new platform menu adapter, using the platform menu
	// and add it to the end of the menu list and platform menubar.
	// ------------------------------------------------------------

	    ODWinMenuAdapter *newMenuAd = new ODWinMenuAdapter;
	    newMenuAd->Init(ev, menuID, _fPlatMenuBar);
	    newMenuAd->AddItemsFromPlatMenuEx(ev, menu, subMenuIDcount,
							subMenuIDinfo);

	    // get the platform menu from the new menu adapter
	    // and insert an popup item for it into the platform menu bar.
	    // --------------------------------------------------------------
	    ODPlatformMenu subMenu = NULL;
	    newMenuAd->GetPlatMenu(ev, &subMenu);

	    BOOL      res;
	    DWORD     err;

	    res = AppendMenu(_fPlatMenuBar, MF_POPUP, (UINT)subMenu, NULL);

	    if (res == TRUE)
		UpdateWinMenuBar(somSelf, ev);
	    else
		err = GetLastError();

	    // set the menu's part, and add it to _fMenus
	    // ------------------------------------------
	    newMenuAd->SetPart(ev, part);

	    _fMenus->AddLast((ElementType) newMenuAd);

	    if (menuID == IDMS_HELP)
		_fHelpMenuExists = kODTrue;
	}

    }
    catch (ODException _exception)
    {
        SetErrorCode(kODNoError);
    }

} /* end AddMenuLastEx */

//-------------------------------------------------------------------------------------
// AddSubMenuBeforeEx [WIN32 only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean SOMLINK ODMenuBarAddSubMenuBeforeEx(ODMenuBar *somSelf,
							 Environment *ev,
							ODMenuID menuID,
							ODMenuItemID subMenuItemID,
							ODPlatformMenu newSubMenu,
							ODMenuID beforeID,
							ODUShort subMenuIDcount,
							ODMenuIDInfo *subMenuIDinfo)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddSubMenuBeforeEx");

    ODBoolean bResult = kODFalse;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
    {
	somSelf->AddMenuBeforeEx(ev, (ODMenuID) subMenuItemID,
				 newSubMenu, kODNULL, beforeID,
				 subMenuIDcount, subMenuIDinfo);
	return kODTrue;
    }

    try {

	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	{
	    bResult = winMenuAd->AddSubMenuBeforeEx(ev, subMenuItemID,
						    newSubMenu, beforeID,
						    subMenuIDcount,
						    subMenuIDinfo);
	    if (bResult == kODTrue)
		UpdateWinMenuBar(somSelf, ev);
	}
    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end AddSubMenuBeforeEx */

//-------------------------------------------------------------------------------------
// AddSubMenuLastEx [WIN32 only]
//-------------------------------------------------------------------------------------
SOM_Scope ODBoolean SOMLINK ODMenuBarAddSubMenuLastEx(ODMenuBar *somSelf,
							Environment *ev,
							ODMenuID menuID,
							ODMenuItemID subMenuItemID,
							ODPlatformMenu newSubMenu,
							ODUShort subMenuIDcount,
							ODMenuIDInfo *subMenuIDinfo)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarAddSubMenuLastEx");

    ODBoolean bResult = kODFalse;

    // TO DO: we should warn callers not to expect this behavior any more...
    // ---------------------------------------------------------------------
    if (menuID == ID_BASEMENUBAR)
    {
	somSelf->AddMenuLastEx(ev, (ODMenuID) subMenuItemID,
				   newSubMenu, kODNULL,
				   subMenuIDcount, subMenuIDinfo);
	return kODTrue;
    }

    try {

	ODWinMenuAdapter* winMenuAd;

	if (GetWinMenuAdapter(somSelf, ev, menuID, &winMenuAd))
	{
	    bResult = winMenuAd->AddSubMenuLastEx(ev, subMenuItemID, newSubMenu,
						  subMenuIDcount, subMenuIDinfo);
	    if (bResult == kODTrue)
		UpdateWinMenuBar(somSelf, ev);
	}
    }
    catch (ODException _exception)
    {
	bResult = kODFalse;
	SetErrorCode(kODNoError);
    }

    return bResult;

} /* end AddSubMenuLastEx */

//-------------------------------------------------------------------------------------
// InitMenuBarEx [PRIVATE] [WIN32 only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarInitMenuBarEx(ODMenuBar *somSelf,
						Environment *ev,
						ODSession* session,
						ODPlatformMenuBar menubar,
						ODUShort subMenuIDcount,
						ODMenuIDInfo *subMenuIDinfo)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarInitMenuBarEx");

    try {

	somSelf->InitRefCntObject(ev);

	_fSession = session;

        _fPlatMenuBar = CreateMenu();
	_fGeneration = 0;
	_fMenus = new OrderedCollection;

	// iterate over platform menus in the platform menubar;
	// and add them to self.
	// ----------------------------------------------------
	ODUShort nMenus = 0;
	DWORD    err;

	if (menubar)
 	    nMenus = GetMenuItemCount(menubar);

	if (nMenus == -1)
	    err = GetLastError();

	for (ODUShort pos = 0; pos < nMenus; pos++)
	{
	    HMENU subMenu   = GetSubMenu(menubar, pos);
	    ODMenuID menuID = 0;

	    if (subMenu)
	    {
		// find the subMenu ID, using subMenuIDinfo[]
		// ------------------------------------------
		for (	ODUShort index = 0;
			((index < subMenuIDcount) && (!menuID));
			index++	)
		{
		    if (subMenu == subMenuIDinfo[index].hMenu)
			menuID = subMenuIDinfo[index].id;
		}

		somSelf->AddMenuLastEx(ev, menuID, subMenu, kODNULL,
					   subMenuIDcount, subMenuIDinfo);

		// set menu text from platform menu
		// --------------------------------
		char menuText[256];

		ODULong nLen = GetMenuString(menubar, pos, menuText, 255,
					     MF_BYPOSITION);

		if (nLen > 0)
		    somSelf->SetMenuText(ev, menuID, menuText);
	    }
	    else
	    {
		err = GetLastError();
	    }
	}

	UpdateWinMenuBar(somSelf, ev);
   }
   catch (ODException _exception)
   {
	SetErrorCode(kODNoError);
   }

} /* end InitMenuBarEx */

#endif // PLATFORM_WIN32

#ifdef _PLATFORM_AIX_
//=====================================================================================
// METHODS FOR AIX ONLY
//=====================================================================================
//-------------------------------------------------------------------------------------
// CreateX11PlatformMenu [AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope ODPlatformMenu  SOMLINK ODMenuBarCreateX11PlatformMenu(ODMenuBar *somSelf,
								  Environment *ev,
								 ODMenuID menuID)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarCreateX11PlatformMenu");

    ODX11MenuAdapter* newMenuAd = kODNULL;

    try {
        newMenuAd = new ODX11MenuAdapter;
        newMenuAd->Init(ev, menuID,_fSession);
    }
    catch (ODException _exception)
    {
        ODDeleteObject(newMenuAd);
        newMenuAd = kODNULL;
        SetErrorCode(kODNoError);
    }

    return (ODPlatformMenu) newMenuAd;

} /* end CreateX11PlatformMenu */

//-------------------------------------------------------------------------------------
// DestroyX11PlatformMenu [AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarDestroyX11PlatformMenu(ODMenuBar *somSelf,
							 Environment *ev,
							ODPlatformMenu menu)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarDestroyX11PlatformMenu");

    ODX11MenuAdapter* killMenu = (ODX11MenuAdapter*) menu;

    ODDeleteObject(killMenu);

} /* end DestroyX11PlatformMenu */

#endif // PLATFORM_AIX

//=====================================================================================
// PRIVATE METHODS FOR ALL PLATFORMS
//=====================================================================================
//-------------------------------------------------------------------------------------
// InitMenuBar [PRIVATE]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarInitMenuBar(ODMenuBar *somSelf,
					      Environment *ev,
					      ODSession* session,
					      ODPlatformMenuBar menubar)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarInitMenuBar");

    #ifdef _PLATFORM_WIN32_
	somSelf->InitMenuBarEx(ev, session, menubar, 0, kODNULL);
    #endif // PLATFORM_WIN32

    try {

	somSelf->InitRefCntObject(ev);

	_fSession = session;

	#ifdef _PLATFORM_AIX_
	    _fMenus = new OrderedCollection;

	    if (menubar)
	    {
		// iterate over menu adapters in the base menubar;
		// and add them to self.
		// ----------------------------------------------------
		ODMenuBarData     *baseBarData  = ODMenuBarGetData(menubar);
		OrderedCollection *baseBarMenus = baseBarData->fMenus;

		OrderedCollectionIterator iter(baseBarMenus);
		for (ODX11MenuAdapter* menuAd = (ODX11MenuAdapter*)iter.First();
		     iter.IsNotComplete();
		     menuAd = (ODX11MenuAdapter*) iter.Next())
		{
		    ODX11MenuAdapter* newMenuAd = menuAd->Copy(ev);
		    _fMenus->AddLast((ElementType) newMenuAd);
		}

		_fHelpMenuExists = baseBarData->fHelpMenuExists;
	    }

	#endif // PLATFORM_AIX

	#ifdef _PLATFORM_OS2_
            _fPlatMenuBar = WinCreateMenu(HWND_OBJECT, NULLHANDLE);
	    _fGeneration = 0;
	    _fMenus = new OrderedCollection;

	    // copy style bits from base menu
	    // ------------------------------
	    ULONG ulStyle = WinQueryWindowULong((HWND)menubar, QWL_STYLE);
	    ulStyle |= MS_ACTIONBAR | WS_CLIPSIBLINGS;
	    ulStyle &= ~WS_SAVEBITS;
	    WinSetWindowULong((HWND)_fPlatMenuBar, QWL_STYLE, ulStyle);

	    // copy ID and font name from base menu
	    // ------------------------------------
	    const ULONG nMaxPath = 288;
	    char  szFontName[nMaxPath];

	    USHORT usID = (USHORT)WinQueryWindowUShort(menubar,QWS_ID);
	    WinSetWindowUShort(_fPlatMenuBar, QWS_ID, usID);
	    WinQueryPresParam(menubar, PP_FONTNAMESIZE, 0L, NULL,
				nMaxPath, (PPVOID)szFontName, 0);
	    WinSetPresParam(_fPlatMenuBar, PP_FONTNAMESIZE,
				strlen(szFontName)+1, (PVOID)szFontName);

	    // iterate over platform menus in the platform menubar;
	    // and add them to self.
	    // ----------------------------------------------------
	    ODUShort nMenus = SHORT1FROMMR(WinSendMsg(menubar, MM_QUERYITEMCOUNT,
						      0L, 0L));
	    MRESULT mres;

	    for (ODUShort pos = 0; pos < nMenus; pos++)
	    {
		ODMenuID menuID = SHORT1FROMMR(WinSendMsg(menubar,
							  MM_ITEMIDFROMPOSITION,
							  MPFROMSHORT(pos), 0L));
		ODPlatformMenuItem menuItem;

		mres = WinSendMsg(menubar, MM_QUERYITEM, MPFROM2SHORT(menuID, FALSE),
							 MPFROMP(&menuItem));

		if (menuItem.hwndSubMenu)
		    somSelf->AddMenuLast(ev, menuID, menuItem.hwndSubMenu,
						     kODNULL);

		// set menu text from platform menu
		// --------------------------------
		char *menuText;

		ODULong nLen = SHORT1FROMMR(WinSendMsg(menubar, MM_QUERYITEMTEXTLENGTH,
							MPFROMSHORT(menuID), 0L));

		if (nLen > 0)
		{
		    menuText = new char[nLen+1];

		    mres = WinSendMsg(menubar, MM_QUERYITEMTEXT,
				      MPFROM2SHORT(menuID, (nLen+1)), menuText);

		    somSelf->SetMenuText(ev, menuID, menuText);

		    delete menuText;
		}
	    }
	#endif // PLATFORM_OS2

	#ifdef _PLATFORM_MACINTOSH_
	    _fSystemMenus = menuBar;
	#endif // _PLATFORM_MACINTOSH

   }
   catch (ODException _exception)
   {
	SetErrorCode(kODNoError);
   }

} /* end InitMenuBar */

//-------------------------------------------------------------------------------------
// SetGeneration [PRIVATE]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarSetGeneration(ODMenuBar *somSelf,
						Environment *ev,
						ODULong generation)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarSetGeneration");

    _fGeneration = generation;

} /* end SetGeneration */

//-------------------------------------------------------------------------------------
// GetGeneration [PRIVATE]
//-------------------------------------------------------------------------------------
SOM_Scope ODULong  SOMLINK ODMenuBarGetGeneration(ODMenuBar *somSelf,
						   Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarGetGeneration");

    return _fGeneration;

} /* end GetGeneration */

//=====================================================================================
// OVERRIDDEN SOM METHODS
//=====================================================================================

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
//-------------------------------------------------------------------------------------
// somInit [OVERRIDE] [OS/2, Windows, and AIX only]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarsomInit(ODMenuBar *somSelf)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarsomInit");

    parent_somInit(somSelf);

    _fSession       = kODNULL;
    _fGeneration    = 0;

    _fHelpMenuExists = kODFalse;

    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
	_fPlatMenuBar   = kODNULL;
	_fAccelsToAdd = kODFalse;
	_fNumberofAccels = 0;
    #endif // PLATFORM_OS2, PLATFORM_WIN32

    _fDefaultAccelTable = NULLHANDLE;
    _fNewAccelTable     = NULLHANDLE;

} /* end somInit */
#endif // _PLATFORM_OS2, PLATFORM_WIN32, PLATFORM_AIX

//-------------------------------------------------------------------------------------
// somUninit [OVERRIDE]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarsomUninit(ODMenuBar *somSelf)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarsomUninit");

    Environment *ev = somGetGlobalEnvironment();

    // Make sure this isn't the current menubar
    // ----------------------------------------
    if (_fSession)
    {
	ODWindowState* winStat = _fSession->GetWindowState(ev);

	if (winStat) {
	   ODMenuBar* currentMenuBar = winStat->AcquireCurrentMenuBar(ev);

	   if (currentMenuBar == somSelf)
		winStat->SetCurrentMenuBar(ev, kODNULL);

	   currentMenuBar->Release(ev);
	}
    }

    #ifdef _PLATFORM_OS2_
	while (_fMenus->Count() != 0);
	{
	    ODOS2MenuAdapter* os2menuAd = (ODOS2MenuAdapter*)_fMenus->First();
	    _fMenus->Remove((ElementType) os2menuAd);
	    delete os2menuAd;
	}
	WinDestroyWindow(_fPlatMenuBar);
	if (_fDefaultAccelTable)
	    WinDestroyAccelTable(_fDefaultAccelTable);
	if (_fNewAccelTable)
	    WinDestroyAccelTable(_fNewAccelTable);
    #endif // PLATFORM_OS2

    #ifdef _PLATFORM_WIN32_
	while (_fMenus->Count() != 0);
	{
	    ODWinMenuAdapter* winMenuAd = (ODWinMenuAdapter*)_fMenus->First();
	    _fMenus->Remove((ElementType) winMenuAd);
	    delete winMenuAd;
	}
	DestroyWindow(_fPlatMenuBar);
	if (_fDefaultAccelTable)
	    DestroyAcceleratorTable(_fDefaultAccelTable);
	if (_fNewAccelTable)
	    DestroyAcceleratorTable(_fNewAccelTable);
    #endif // PLATFORM_WIN32

    #ifdef _PLATFORM_AIX_
	while (_fMenus->Count() != 0);
	{
	    ODX11MenuAdapter* x11menuAd = (ODX11MenuAdapter*)_fMenus->First();
	    _fMenus->Remove((ElementType) x11menuAd);
	    delete x11menuAd;
	}
    #endif // PLATFORM_AIX

    #ifdef _PLATFORM_MACINTOSH_
	ODDeleteObject(_fCommandTable);

	if (_fOtherMenus)
	{
	    TRY
		OrderedCollectionIterator* iter = _fOtherMenus->CreateIterator();
		for (ElementType t = iter->First(); iter->IsNotComplete(); t = iter->Next())
		{
		    ODDeleteObject(t);
		}
		ODDeleteObject(iter);
		ODDeleteObject(_fOtherMenus);
	    CATCH_ALL
		// Ignore any exceptions
	    ENDTRY
	}
    #endif // PLATFORM_MACINTOSH

    parent_somUninit(somSelf);

} /* end somUninit */

//-------------------------------------------------------------------------------------
// Purge [OVERRIDE]
//-------------------------------------------------------------------------------------
SOM_Scope ODSize  SOMLINK ODMenuBarPurge(ODMenuBar *somSelf, 
                                          Environment *ev, ODSize size)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarPurge");

    #if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
	return (ODMenuBar_parent_ODRefCntObject_Purge(somSelf, ev, size));
    #endif // PLATFORM_OS2, PLATFORM_WIN32, PLATFORM_AIX

    #ifdef _PLATFORM_MACINTOSH_
	ODUnused(size);
	ODSize purgeSize = 0;

	SOM_TRY
	    purgeSize = parent_Purge(somSelf, ev, size);
	SOM_CATCH_ALL
	SOM_ENDTRY

    #endif // PLATFORM_MACINTOSH

} /* end Purge */

//-------------------------------------------------------------------------------------
// Release [OVERRIDE]
//-------------------------------------------------------------------------------------
SOM_Scope void  SOMLINK ODMenuBarRelease(ODMenuBar *somSelf, 
                                          Environment *ev)
{
    ODMenuBarData *somThis = ODMenuBarGetData(somSelf);
    ODMenuBarMethodDebug("ODMenuBar","ODMenuBarRelease");

    try {

	parent_Release(somSelf, ev);

    #ifdef _PLATFORM_MACINTOSH_
	if (somSelf->GetRefCount(ev) == 0)
	    somSelf->somFree();
    #endif // _PLATFORM_MACINTOSH

    }
    catch (ODException _exception)
    {
	SetErrorCode(kODNoError);
    }

} /* end Release */
