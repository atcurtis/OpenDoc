//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   DialogScriptData
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
// @(#) 1.8 com/src/utils/include/DlogUtil.h, odutils, od96os2, odos29646d 7/15/96 18:00:04 [ 11/15/96 15:28:58 ]
/*
	File:		DlogUtil.h

	Contains:	function declarations dialog utility functions

	Owned by:	Tantek ‚elik

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/

#ifndef _DLOGUTIL_
#define _DLOGUTIL_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __LISTS__
#include <Lists.h>
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef SOM_Module_OpenDoc_Errors_defined
#include "ErrorDef.xh"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __SCRIPT__
#include "Script.h"
#endif
#endif // _PLATFORM_MACINTOSH_

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif


#ifdef __cplusplus
#define DEFAULT(V) =(V)
extern "C" {
#else
#define DEFAULT(V)
#endif

//==============================================================================
// Classes Used in Interface
//==============================================================================

class ODSession;
class ODMenuBar;
class DialogScriptData;

//==============================================================================
// Psuedo-items representing arrow keys and PageUp etc.
//==============================================================================

const char kEnterKey		= 0x03;
const char kReturnKey		= 0x0D;
const char kEscapeKey		= 0x1B;
const char kUpArrowKey		= 0x1E;
const char kDownArrowKey	= 0x1F;

const ODSShort kODUpArrowItem = -1000;
const ODSShort kODDownArrowItem = -1001;
const ODSShort kODPageUpArrowItem = -1002;
const ODSShort kODPageDownArrowItem = -1003;
const ODSShort kODHomeArrowItem = -1004;
const ODSShort kODEndArrowItem = -1005;

#if !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_AIX_)
//==============================================================================
// Dialog Utility Macros
//==============================================================================

#define ODDisposeRoutineDescriptor(r) \
	if ( !r ) ; else DisposeRoutineDescriptor(r)

#define ODDisposeDialog(dlg) \
	if ( !dlg ) ; else DisposeDialog(dlg)

#if ODDebug
	inline void ASSERT_CONTROL_ITEM( short itemType )
	{
		itemType &= ~kItemDisableBit;
		ASSERT(itemType>=ctrlItem && itemType<=ctrlItem+resCtrl,kODErrAssertionFailed);
	}
#else
	#define ASSERT_CONTROL_ITEM(T)	do{ } while(0)
#endif

#define IS_ARROW_KEY(key)														\
	((ODBoolean) (((key) >= kODEndArrowItem) && ((key) <= kODUpArrowItem)))		\

//==============================================================================
// Dialog Utility Functions
//==============================================================================

DialogPtr ODGetNewDialog( Environment*, short id, ODSession*,
							ODBoolean defaultButtons DEFAULT(kODTrue) );
	// Calls GetNewDialog, with preflighting, and sets things up so the filter
	// proc (below) will work. If defaultButtons is true, will call SetDlog-
	// DefaultItem to set up the standard OK and Cancel buttons.

ODSShort ShowAlert(Environment *ev, 
					ODSShort alertID, 
					ModalFilterUPP modalFilter, 
					ODSession *session);
	// Calls Alert, with preflighting, and sets things up so standard OpenDoc 
	// filter procs (below) will work.  This function opens the current library's
	// resource fork, so it must not be called when a resource fork is already
	// open.

pascal Boolean ODDialogFilterProc( DialogPtr, EventRecord *, short *item );
	// FilterProc to use for dialogs. Forwards update/activate/null/os events
	// to Dispatcher. Handles std keyboard equivs and movable-modal dialogs.
	// You MUST have created the dialog with ODGetNewDialog!

ModalFilterUPP GetODDialogFilter( );
	// Returns global UPP for above filter proc; can be passed directly to
	// ModalDialog() or Alert(). Do not dispose this UPP!

pascal Boolean ODArrowKeyFilterProc(DialogPtr dialog, EventRecord *event, short *itemHit);
	// Checks for arrow keys, PageUp, PageDown, Home and End keys, and cmd- and option-
	// modified arrows which are their synonyms.  Sets *itemHit to psuedo-items (negative
	// integers) and returns TRUE when successful, otherwise calls ODButtonKeyFilterProc.

ModalFilterUPP GetODArrowKeyFilterProc();
	// Returns global UPP for above filter proc; can be passed directly to
	// ModalDialog(). Do not dispose this UPP!

pascal Boolean ODButtonKeyFilterProc(DialogPtr dialog, EventRecord *event, short *itemHit);
	// Checks for return, enter, escape and cmd-period keys (assumes first item is
	// the OK button and the second item is the cancel button).  Call when 
	// ODOutlineDefaultButtonDrawProc  is employed to outline the default buton.
	// The return and enter keys do not exit the dialog if the first item is inactive.
	// Passes other events to ODDialogFilterProc.

ModalFilterUPP GetODButtonKeyFilterProc( );
	// Returns global UPP for above filter proc; can be passed directly to
	// ModalDialog(). Do not dispose this UPP!

void ODUseCommandKeyStringsResource(short resID);
	// Tells ODButtonKeyFilterProc which STR# resource to use to lookup
	// dialog-specific command keys.  Each string in the STR# resource
	// corresponds by index to each keyable item (button) in the DITL.
	// If the string is "", no command key applies to the item.  Otherwise,
	// the first character of the string is used as the command key code
	// for its corresponding DITL item.  The number of strings in the
	// STR# resource should be less than or equal to the number of items
	// in the DITL resource of the active dialog, or Very Bad Things might
	// happen.

void ODDialogBegin( Environment* ev, ODSession* session, 
					ODMenuBar* currentMenuBar, DialogPtr dialog );
	// Call before ModalDialog, this function removes the Redo menu item so
	// that ModalDialog will know how to enable & disable Cut, Copy & Paste.
	// Must be paired with ODDialogEnd after ModalDialog completes.

void ODDialogEnd();
	// Call to cleanly restore the Redo menu item that was deleted in ODDialogBegin.

pascal void ODOutlineDefaultButtonDrawProc(DialogPtr theDialog, short theItem);
	// User item draw proc that outlines dialog item 1.
	// Unlike SetDialogDefaultItem, the outline is drawn dim if the button is inactive.
	// If you use this draw proc, be sure to pass kODFalse as the defaultButtons
	// parameter to ODGetNewDialog.

UserItemUPP GetODOutlineDefaultButtonDrawProc( );
	// Returns global UPP for above draw proc; can be assigned as an item handle.
	// Do not dispose this UPP!

void ODUseDialogScriptData( DialogScriptData* dsd, DialogPtr dialog );
	// set the global to be used by CheckKeyScriptChangeFilterProc

pascal ODBoolean CheckKeyScriptChangeFilterProc( DialogPtr dialog,
		EventRecord *event, short *itemHit);
	// Working in cooperation with an instance of DialogScriptData, changes
	// the script/font of a dialog's editable text items when necessary
	// to allow a user to enter text in a non-Roman font.  Since Roman
	// fonts cannot display much non-Roman text, and fonts of one non-Roman
	// script often can't display text of another, we only allow users to
	// switch keyscripts once per dialog, and then only if the starting
	// script is Roman.
	// Note: this filter always returns kODFalse, since it never consumes
	// events.  Call it from your own local filter proc, then pass control
	// to any other filters you need.

void EnableOkButton(DialogPtr dlog, ODBoolean enable);
	// Enables or disables the first dialog item and invalidates the area occupied
	// by a default frame around the button.  Should be used to disable the deafult
	// button when ODOutlineDefaultButtonDrawProc is used to outline the button.

pascal void DrawGrayBoxItem(DialogPtr theDialog, short theItem);
pascal void DrawItemFrame(DialogPtr theDialog, short theItem);
	// UserItem procs that draw gray or black boxes around items.

void FlashButtonItem( DialogPtr dialog, short itemHit );
	// Flash the button in the dialog (used to indicate that a command key
	// combination has selected it.

ODBoolean DrawITextInDlogBox( ODIText* itext, const Rect* textRect,
		DialogPtr dlog, ODBoolean tryToTruncate );
	// Given a dialog, a rect and an ODIText*, truncate the text to fit in the
	// rect and then draw it in the dialog.  Return true if any change made to
	// the text (ie. if it was truncated).

void ArrowKeyScrollList( ODSShort arrowItem, ListHandle listH,
		ODSShort pageSize, ODSShort lastEntry );
	// Given an "item" representing an up or down arrow or any of the other keys
	// recognized by ODArrowKeyFilterProc above, a ListHandle indicating the list
	// in which scrolling is taking place, pageSize giving the number of entries to
	// be skipped by a "page down" command, and the zero-based index of the last
	// entry in the list, do the right thing for the key selected.  So far this
	// routine is used for lists embedded in dialogs, and it's up to the caller
	// to determine which list is meant if there is more than one.
	// NOTE that nothing happens currently if no list item is selected.  There needs
	// to be a starting point for scrolling.

void ReplaceIntoString(ODSShort strResourceID,
						ConstStr255Param str0,
						ConstStr255Param str1,
						Str255 destString);
	// Substitutes str0 for all occurances of "^0", and str1 for "^1", in the 
	// string retrieved from the 'STR ' resource identified by the first parameter.
	// Either str0 or str1 may be null, and str0 or str1 may be the same string
	// as destString.

ODIText* GetODITextInd(short resID, short index);
	// Gets string # (index) from the 'STR#' resource given by resID 
	// into an IText* whose script and language codes are those of 
	// the current system script.
	// You MUST first call BeginUsingLibraryResources or CUsingLibraryResources!

ODIText* GetODIText(short resID);
	// Gets the 'STR ' resource given by resID into an IText* whose script
	// and language codes are those of the current system script.
	// You MUST first call BeginUsingLibraryResources or CUsingLibraryResources!

void SetDialogTextStyle(DialogPtr dlg, short finfResourceID,
		ODScriptCode script);
	// Sets the font, styl, and size of the argument dialog according to the
	// 'finf' resource identified by the second parameter.
	// script parameter overrides what's in the finf resource; intended for
	// language kit cases where the default script is Roman but we need to
	// display text Roman fonts can't handle.  Pass smCurrentScript if you
	// want this param ignored.

void SetPopupItemScript(MenuHandle menu, short itemNum, short scriptID);
	// Sets the script of an item in a popup menu, which is assumed to be
	// drawn using the font of the current port.
	// "scriptID" should be a true script code, not an implicit script code


	// This class provided the state required by clients of
	// CheckKeyScriptChangeFilterProc above.  Typically they'll need
	// to instantiate a member of the class, pass it to ODUseDialogScriptData
	// (also above), and then create and use a custom FilterProc that
	// calls CheckKeyScriptChangeFilterProc.  Since CheckKeyScriptChangeFilterProc
	// always returns false, the custom filter proc should be prepared to
	// call some other filter proc in addition.
const short kMaxNumItems = 32;	// max number of items in a dialog
const short ksmUninited = -4;
class DialogScriptData
{
  public:
	DialogScriptData();
	ODVMethod	~DialogScriptData();
	ODMethod	void SetScriptAndLockKeyboard();
	ODMethod	void SetDialog(DialogPtr dlg)	{ fDialog = dlg; }
	ODMethod	void SetScriptAndLockKeyboard( ODScriptCode script );
	ODMethod	ODBoolean ScriptChanged()	{ return fMustUnlock; }
	ODMethod	ODBoolean ItemRedrawn( short item );
	ODMethod	void SetRedrawn( short item );
	ODMethod	ODScriptCode Script()	{ return fScript; }
	ODMethod	ODLangCode Language()
			{ return GetScriptVariable(fScript, smScriptLang); }

  protected:
	ODMethod	void ReleaseLockout();

  private:
	ODScriptCode fScript;
	ODBoolean fMustUnlock;
	ODULong fItemRedrawn;			// bit vector of items changed
	DialogPtr fDialog;
};

#endif // !defined (_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_AIX_)

#ifdef __cplusplus
}
#endif

#endif // _DLOGUTIL_
