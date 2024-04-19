/* @(#)Z 1.14 com/src/ui/ExcFocus.cpp, odui, od96os2, odos29646d 96/11/15 15:28:03 (96/09/03 12:21:13) */
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
	File:           ExcFocus.cpp

	Contains:       Implementation of ODExclusiveFocusModule

	Owned by:       Richard Rodseth

	Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<24>    10/24/95        jpa             1293441: Eat exceptions in
									ResolveClipboardPromises
		<23>     10/8/95        TJ              Fixes Recomended by Refball
		<22>     10/3/95        TJ              Changes done by RefBall Team
		<21>      8/3/95        RR              #1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<20>     6/28/95        RR              1242642 BB Mostly ref counting. Release
									border shape
		<19>     6/25/95        T‚              1242642 BB: Turn on ODDebug warning if
									refcount is wrong in
									ODRefCntObjectsomUninit.
		<18>     5/31/95        RR              #1251403 Release after AcquirePart
		<17>     5/26/95        RR              #1251403: Multithreading naming support
		<16>     5/17/95        RR              #1250135/1250137/1250143 Getters increment
									refcount
		<15>     5/16/95        CC              1248632, 1244940: SetFocusOwnership:
									Externalize the current clipboard draft to
									force resolution of promises.
		<14>     5/10/95        JBS             1241913 ODPart::AdjustBorderShape is not
									called with null shape
		<13>     4/28/95        RR              1211085 Remove 5$ comments
		<12>      4/7/95        RR              #1216618 Added SOM_TRY etc.
		<11>      4/6/95        RR              #1220104 Use of ODObjectsAreEqual, #1211085
									Remove 5$ comments
		<10>     3/13/95        NP              1228158: Always call UseInputWindow when
									losing keystroke focus.
		 <9>      3/7/95        RR              #1220929 Added cache for selection focus
		 <8>      3/1/95        NP              1187682: Call UseInputWindow when keystroke
									focus is lost.
		 <7>    10/27/94        RR              Call FocusLost in TransferFocus #1192168
		 <6>    10/18/94        RR              Added fSession. Detect change in selection
									focus. Override BRF, CRF, ARF.
		 <5>     9/20/94        RR              When assigning a new frame to fFrame, call
									Acquire on the new frame BEFORe
									calling Release on the old, in case they're
									the same frame.
		 <4>      9/2/94        RR              #1184275 Pass transferringFrame and
									newOwner to focusModule::TransferFocus
		 <3>     8/24/94        RR              Fixed ref counting in SetFocusOwnership
		 <2>     8/23/94        RR              Call Acquire and Release
		 <1>     6/23/94        RR              first checked in

	To Do:
	In Progress:
		
*/


#define ODExclusiveFocusModule_Class_Source
#define VARIABLE_MACROS
/*
class ODHandle; */  // CJC
/* class ODPlatformWindow; */  // CJC
#include <ExcFocus.xih>

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef SOM_ODFrame_xh
#include "Frame.xh"
#endif

#ifndef SOM_Module_OpenDoc_Foci_defined
#include "Foci.xh"
#endif

#ifndef _ODSESSN_
#include "ODSessn.xh"
#endif

#ifndef SOM_ODPart_xh
#include "Part.xh"
#endif

#ifndef SOM_ODFrame_xh
#include "Frame.xh"
#endif

#ifdef _PLATFORM_WIN32_
#include "facet.xh"
#endif

#ifndef SOM_ODFrameFacetIterator_xh
#include "FrFaItr.xh"
#endif

#ifndef SOM_ODArbitrator_xh
#include <Arbitrat.xh>
#endif

#ifndef SOM_ODClipboard_xh
#include <Clipbd.xh>
#endif

#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)
#ifndef __TEXTSERVICES__
#include <TextServices.h>       // Platform-dependent
#endif
#endif


#ifdef _PLATFORM_OS2_
#define INCL_ODAPI
#include <os2.h>

#ifndef _WINDOW_
#include "Window.xh"
#endif

#ifndef _FACET_
#include "Facet.xh"
#endif

#ifndef _PASCLSTR_
#include "PasclStr.h"
#endif

#ifndef _STDTYPIO_
#include "StdTypIO.h"
#endif

#ifndef _BNDNSUTL_
#include "BndNSUtl.h"
#endif

#ifndef _ODUTILS_
#include "odutils.h"
#endif
#endif  // _PLATFORM_OS2_

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif


#ifdef _PLATFORM_WIN32_
#ifndef _ODUTILS_
#include <odutils.h>
#endif
#include <olewrap.h>
#include <olemgrw.h>
#include <docMgr.xh>
#endif

//==============================================================================
// Local function declarations
//==============================================================================
#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)
ODStatic void ResolveClipboardPromises(Environment *ev,
				ODFrame* selectionFrame,
				ODSession* session);
#endif

#ifdef _UNICODE_
UniChar*        UniStr;
char*           CharStr;
ODConvertText*  convert;
UconvObject     co;
#endif

//==============================================================================
// Class ODExclusiveFocusModule
//==============================================================================

SOM_Scope void  SOMLINK ODExclusiveFocusModuleInitExclusiveFocusModule(ODExclusiveFocusModule *somSelf, Environment *ev,
		ODSession* session)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","InitExclusiveFocusModule");

	SOM_TRY
	
		/* Moved from somInit. SOM itself sets fields to zero
		_fFrame = kODNULL;
		_fSession = kODNULL;
		_fSelectionFocus = 0;
		_fKeyFocus = 0;
		*/
		
		somSelf->InitFocusModule(ev, session);
		_fSession = session;
		_fSelectionFocus = session->Tokenize(ev,kODSelectionFocus);
		_fKeyFocus = _fSession->Tokenize(ev, kODKeyFocus);

	SOM_CATCH_ALL
	SOM_ENDTRY
}


SOM_Scope void  SOMLINK ODExclusiveFocusModulesomUninit(ODExclusiveFocusModule *somSelf)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","somUninit");

    parent_somUninit(somSelf);
}

SOM_Scope ODBoolean  SOMLINK ODExclusiveFocusModuleIsFocusExclusive(ODExclusiveFocusModule *somSelf, Environment *ev,
		ODTypeToken focus)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","IsFocusExclusive");

	ODUnused(focus);
	return kODTrue;
}

SOM_Scope void  SOMLINK ODExclusiveFocusModuleSetFocusOwnership(ODExclusiveFocusModule *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* frame)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","SetFocusOwnership");
#if !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_WIN32_)
	ODFacet* facet = kODNULL;
	ODFrameFacetIterator* facets = kODNULL;
#endif
	SOM_TRY
	
		if (focus == _fSelectionFocus)
		{
			if (_fFrame)
				_fFrame->InvalidateActiveBorder(ev);
#if !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_WIN32_)
				TempODFrame contFrame = _fFrame->AcquireContainingFrame(ev); // -- T‚ tempobj'd
				if ((ODFrame*) contFrame )
				{
					TempODPart contPart = contFrame->AcquirePart(ev); // -- T‚ tempobj'd
					facets = _fFrame->CreateFacetIterator(ev);
					for ( facet = facets->First(ev);
					facets->IsNotComplete(ev);
							facet = facets->Next(ev) )
					{
						ODShape* border = contPart->AdjustBorderShape(ev, facet, kODNULL);
						ODReleaseObject(ev, border);
					}
					ODDeleteObject(facets);
				}
			}
			// Note:  window->Update(ev);
			if (frame)
				frame->DrawActiveBorder(ev);
			ResolveClipboardPromises(ev, _fFrame, _fSession);
#endif //  !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_WIN32_)
#ifdef _PLATFORM_OS2_  // [124872] - ced  OS2 TASK LIST MODIFICATION CODE
     {
#if 0  // was not implemented in OS2 code
// if selection focus changes update the Title bar and Window List
// with the active editor name.
	 USHORT nopreferredparteditor, rc;
	 ULONG size=0;
	// Str255 edt;
	 CHAR   string[256];
	 CHAR * edt, *str ;
	 ODName * name;
//       HWND hwndplat = _fSession->GetPlatformWindow(ev);                          // ced
	 TempODWindow aWindow = frame->AcquireWindow(ev);
	 HWND hwndplat = aWindow->GetPlatformWindow(ev); // ced

	 ODNameSpaceMana ger*  nsm = _fSession->GetNameSpaceManager(ev);
	 TempODPart party1 = frame->AcquirePart(ev);
	 ODStorageUnit * psu = party1->GetStorageUnit(ev);
	 ODType  pEdit =(ODType) ODGetISOStrProp(ev,psu, kODPropPreferredEditor,kODEditor,kODNULL,&size);
//                        GetPreferredEditorForPart(psu,&nopreferredparteditor);
	 rc = GetUserEditorFromEditor(nsm, pEdit,&name);
#ifdef _UNICODE_
      convert = ConvertFromUnicode(((ODPlainITextData *)name->text._buffer)->theText,
				   CharStr,
				   UniStrlen(((ODPlainITextData *)name->text._buffer)->theText),
				   co);
      edt = CharStr;
#else
      edt =  ((ODPlainITextData *)name->text._buffer)->theText;
#endif
// get the root part
      ODFrame * contFrame, * newFrame;
      SWCNTRL swctl;
      newFrame = frame;
      do {
	 contFrame = newFrame;
	 newFrame = contFrame->AcquireContainingFrame(ev);

      } while ( newFrame ); /* enddo */
      TempODPart party2 = contFrame->AcquirePart(ev);
// get window list data
      HSWITCH  hswitch = WinQuerySwitchHandle(hwndplat,0);
      WinQuerySwitchEntry ( hswitch, &swctl);
      strcpy (string,swctl.szSwtitle);
      USHORT i=0;
      while (string[i] != '\0') {
	if (string[i]== '-' ) {
	   if ((string[i-1]==' ')&&(string[i+1]==' ')) {
	      break;
	   } /* endif */
	} /* endif */
	i++;
      } /* endwhile */
      string[i] = '\0';

//    get part name
      ODName * pname = kODNULL;            // [127087]
      pname = ODGetPOName(ev,party2,pname);    // [127087]
		if (pname != kODNULL)
			{
			str =  GetCStringFromIText(pname);   // [127087]
			strcat(string, " - ");
			strcat(string,str);
	 DisposeIText(pname);                 // [127087]
			}
		strcat(string, " - ");
		strcat(string,edt);
	   // change the Window List Text
      strcpy(swctl.szSwtitle,string);
//    WinChangeSwitchEntry (hswitch, &swctl);
//      WinSetWindowText(hwndplat,string);
#endif // #if 0
      frame->DrawActiveBorder(ev);
    }
      frame->DrawActiveBorder(ev);
#endif // _PLATFORM_OS2

#ifdef _PLATFORM_WIN32_
 // ********* OLE Interop start ******
  TempODPart part = frame->AcquirePart(ev);
  OleWrapper* wrapper = ODOLEManager::GetShell()->GetDocumentManagerOfDraft(ev, part->GetStorageUnit(ev)->GetDraft(ev))->GetOleWrapper(ev);
  if (wrapper) {
     if ((wrapper->fWFlg & WFLG_UIACTIVATED) && (wrapper->fActiveDocumentWindow == wrapper->fRootDocumentWindow))
          wrapper->ActiveFocusChanging(frame);
  }
 // **********OLE Interop end
   if(frame)
      frame->DrawActiveBorder(ev);
#endif // _PLATFORM_WIN32_

  }  // end for - if (focus == _fSelectionFocus)
		
#ifdef _PLATFORM_OS2_  // [124960] - ced
  if (focus == _fKeyFocus)
  {
      /* Restore PM focus to docshell client window. */
      TempODWindow window = frame->AcquireWindow(ev);
      if (window)
      {
	 HWND hwndFrame, hwnd, hwndFocus;
	 hwndFrame = window->GetPlatformWindow(ev);
	 if (WinQueryActiveWindow(HWND_DESKTOP) == hwndFrame)
	 {
	    hwnd = WinWindowFromID(hwndFrame, FID_CLIENT);
	    if (!hwnd)
	       /* No client window!  Odd.  Just use hwndFrame then. */
	       hwnd = hwndFrame;
	    /*
	     * If the window that currently has the PM focus is a child of one of
	     * the facet windows that belongs to the frame receiving the key focus,
	     * then do nothing.  Otherwise, set the PM focus to the shell client
	     * window.
	     */
	    hwndFocus = WinQueryFocus(HWND_DESKTOP);
	    if (hwndFocus != hwnd)
	    {
	       ODFacet* facet = kODNULL;
	       while (hwndFocus && hwndFocus != hwnd && hwndFocus != HWND_DESKTOP)
	       {
		  if ((facet = _ODFacet->clsGetFacetFromHWND(ev, hwndFocus)) != kODNULL)
		     break;
		  hwndFocus = WinQueryWindow(hwndFocus, QW_PARENT);
	       }
	       if (!(facet && facet->GetFrame(ev) == frame))
		  WinSetFocus(HWND_DESKTOP, hwnd);
	    }
	 }
      }
  }
#endif // _PLATFORM_OS2_
#ifdef _PLATFORM_WIN32_

  if (focus == _fKeyFocus)
  {
      /* Restore Window focus to docshell client window. */
      TempODWindow window = frame->AcquireWindow(ev);
      if (window)
      {
	 HWND hwndFrame, hwnd, hwndFocus;
	 hwndFrame = window->GetPlatformWindow(ev);
//         if (WinQueryActiveWindow(HWND_DESKTOP) == hwndFrame)
	 // gets Active window handle for this thread - and compares to current frame handle?
	 if (GetActiveWindow() == hwndFrame)  // NT/95 call
	 {
#ifndef _PLATFORM_WIN32_  // don't need to get client in NT use frame
//            hwnd = WinWindowFromID(hwndFrame, FID_CLIENT); /* gets client window handle */
	    /* is there a win32 api to get handle to client ???????? call below gets child not client */
	    hwnd = GetWindow(hwndFrame, GW_CHILD);   // NT/95 call try and get child window if one exists
	    if (!hwnd)
	       /* No client window!  Odd.  Just use hwndFrame then. */
#endif // _PLATFORM_WIN32_
	       hwnd = hwndFrame;
	    /*
	     * If the window that currently has the PM focus is a child of one of
	     * the facet windows that belongs to the frame receiving the key focus,
	     * then do nothing.  Otherwise, set the PM focus to the shell client
	     * window.
	     */
//            hwndFocus = WinQueryFocus(HWND_DESKTOP);
	    // retrieves the handle of the keyboard focus window associated with the thread that called the function.
	    hwndFocus = GetFocus(); // NT/95 call
	
//#ifndef _PLATFORM_WIN32_ // *** work around CJC - clsGetFacetFromHWND not implemented yet.
//            if (hwndFocus != hwnd)   // CJC if window with focus != this one(current one)
//                SetFocus(hwnd);  // NT/95 call
//#else
	    if (hwndFocus != hwnd)   // CJC if window with focus != this one(current one)
	    {
	       ODFacet* facet = kODNULL;
		 // (not null) && (focus != current) && (focus != owning window)
	       while (hwndFocus && hwndFocus != hwnd && hwndFocus != HWND_DESKTOP)
	       {
		  if ((facet = _ODFacet->clsGetFacetFromHWND(ev, hwndFocus)) != kODNULL)
		     break;
		  //                  hwndFocus = WinQueryWindow(hwndFocus, QW_PARENT);
		  hwndFocus = GetParent(hwndFocus); // NT/95 call - get parent handle of hwndFocus
	       }
		// if (face != NULL) && facet Frame == current frame) then set focus
	       if (!(facet && facet->GetFrame(ev) == frame))
		  SetFocus(hwnd);  // NT/95 call
		//                  WinSetFocus(HWND_DESKTOP, hwnd);
	    }
//#endif
	 }
      }
  }
#endif // _PLATFORM_WIN32_

		if (frame)
			frame->Acquire(ev);
		if (_fFrame)
			_fFrame->Release(ev);
		_fFrame = frame;

	SOM_CATCH_ALL
	SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODExclusiveFocusModuleUnsetFocusOwnership(ODExclusiveFocusModule *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* frame)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","UnsetFocusOwnership");

	SOM_TRY
	if (focus == _fKeyFocus)
	{
#ifdef _PLATFORM_OS2_  // [124960] - ced
      /* Restore PM focus to docshell client window. */
      TempODWindow window = frame->AcquireWindow(ev);
      if (window)
      {
	 HWND hwndFrame, hwnd;
	 hwndFrame = window->GetPlatformWindow(ev);
	 if (WinQueryActiveWindow(HWND_DESKTOP) == hwndFrame)
	 {
	    hwnd = WinWindowFromID(hwndFrame, FID_CLIENT);
	    if (!hwnd)
	       /* No client window!  Strange.  Just use hwndFrame then. */
	       hwnd = hwndFrame;

	    WinSetFocus(HWND_DESKTOP, hwnd);
	 }
      }
#endif
// ** The following is the windows code but it is not complete
// due to the frame and facet design dependency.
#if !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_WIN32_)
      /* Restore focus to docshell client window. */
      TempODWindow window = frame->AcquireWindow(ev);
      if (window)
      {
	 HWND hwndFrame, hwnd;
	 hwndFrame = window->GetPlatformWindow(ev);
	 if (GetActiveWindow() == hwndFrame)  // NT/95 call
//         if (WinQueryActiveWindow(HWND_DESKTOP) == hwndFrame)
	 {
//            hwnd = WinWindowFromID(hwndFrame, FID_CLIENT);
//            hwnd = GetWindow(hwndFrame, GW_CHILD);   // NT/95 call try and get child window if one exists
//            if (!hwnd)
	       /* No client window!  Strange.  Just use hwndFrame then. */
	       hwnd = hwndFrame;

	    SetFocus(hwnd);
//            WinSetFocus(HWND_DESKTOP, hwnd);
	 }
      }
#endif
#if !defined(_PLATFORM_OS2_) && !defined(_PLATFORM_WIN32_)
			const TSMDocumentID     kAnyDoc = kODNULL;
		
			// Make sure to set this back to a safe state.
			UseInputWindow(kAnyDoc, kODTrue);
#endif
		}
	
		if (ODObjectsAreEqual(ev, frame, _fFrame))
		{
			if (focus == _fSelectionFocus)
			{
				if ( _fFrame )
					_fFrame->InvalidateActiveBorder(ev);
				// Note: window->Update(ev);
#ifdef _PLATFORM_WIN32_
   // ********* OLE Interop start ******

  TempODPart part = frame->AcquirePart(ev);
  OleWrapper* wrapper = ODOLEManager::GetShell()->GetDocumentManagerOfDraft(ev, part->GetStorageUnit(ev)->GetDraft(ev))->GetOleWrapper(ev);

  if (wrapper) {
      if ((wrapper->fWFlg & WFLG_UIACTIVATED) && (wrapper->fActiveDocumentWindow == wrapper->fRootDocumentWindow))
           wrapper->ActiveFocusChanging(NULL);
  }
   // **********OLE Interop end
#endif
			}
		
			if (_fFrame)
				_fFrame->Release(ev);
			_fFrame = kODNULL;
		}

	SOM_CATCH_ALL
	SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODExclusiveFocusModuleTransferFocusOwnership(ODExclusiveFocusModule *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* transferringFrame,
		ODFrame* newOwner)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","ODExclusiveFocusModuleTransferFocusOwnership");

	SOM_TRY

		TempODFrame oldOwner = somSelf->AcquireFocusOwner(ev, focus);
		somSelf->SetFocusOwnership(ev, focus, newOwner);
		
		if (oldOwner && (oldOwner != transferringFrame))
		{
			TempODPart part = oldOwner->AcquirePart(ev);
			part->FocusLost(ev, focus, oldOwner);
		}
		if (newOwner && (newOwner != transferringFrame))
		{
			TempODPart part = newOwner->AcquirePart(ev);
			part->FocusAcquired(ev, focus, newOwner);
		}

	SOM_CATCH_ALL
	SOM_ENDTRY
}

SOM_Scope ODFrame*  SOMLINK ODExclusiveFocusModuleAcquireFocusOwner(ODExclusiveFocusModule *somSelf, Environment *ev,
		ODTypeToken focus)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","AcquireFocusOwner");

	ODUnused(focus);
	if (_fFrame) {
		SOM_TRY
			_fFrame->Acquire(ev);
		SOM_CATCH_ALL
		SOM_ENDTRY
	}
	return _fFrame;
}

SOM_Scope ODBoolean  SOMLINK ODExclusiveFocusModuleBeginRelinquishFocus(ODExclusiveFocusModule *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* requestingFrame)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","ODExclusiveFocusModuleBeginRelinquishFocus");

	ODBoolean canRelinquish = kODTrue;
	
	SOM_TRY

		TempODFrame currentOwner = somSelf->AcquireFocusOwner(ev, focus);
	
		if (currentOwner && (!ODObjectsAreEqual(ev, currentOwner, requestingFrame)))
		{
			TempODPart part = currentOwner->AcquirePart(ev); // -- T‚ tempobj'd
			if ((ODPart*)part)
				canRelinquish = part->BeginRelinquishFocus(ev, focus, currentOwner, requestingFrame);
		}

	SOM_CATCH_ALL
	SOM_ENDTRY
	
	return canRelinquish;
}

SOM_Scope void  SOMLINK ODExclusiveFocusModuleCommitRelinquishFocus(ODExclusiveFocusModule *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* requestingFrame)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","ODExclusiveFocusModuleCommitRelinquishFocus");

	SOM_TRY

		TempODFrame currentOwner = somSelf->AcquireFocusOwner(ev, focus);
		
		if ((currentOwner)  && (!ODObjectsAreEqual(ev, currentOwner,requestingFrame)))
		{
			TempODPart part = currentOwner->AcquirePart(ev); // -- T‚ tempobj'd
			if ((ODPart*)part)
				part->CommitRelinquishFocus(ev, focus, currentOwner, requestingFrame);
		}

	SOM_CATCH_ALL
	SOM_ENDTRY
}

SOM_Scope void  SOMLINK ODExclusiveFocusModuleAbortRelinquishFocus(ODExclusiveFocusModule *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFrame* requestingFrame)
{
    ODExclusiveFocusModuleData *somThis = ODExclusiveFocusModuleGetData(somSelf);
    ODExclusiveFocusModuleMethodDebug("ODExclusiveFocusModule","ODExclusiveFocusModuleAbortRelinquishFocus");

	SOM_TRY

		TempODFrame currentOwner = somSelf->AcquireFocusOwner(ev, focus);
		if (currentOwner && (!ODObjectsAreEqual(ev, currentOwner,requestingFrame)))
		{
			TempODPart part = currentOwner->AcquirePart(ev); // -- T‚ tempobj'd
			if ((ODPart*)part)
				part->AbortRelinquishFocus(ev, focus, currentOwner,requestingFrame);
		}

	SOM_CATCH_ALL
	SOM_ENDTRY
}


#if !defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_OS2_)
ODStatic void ResolveClipboardPromises(Environment *ev,
				ODFrame* selectionFrame,
				ODSession* session)
{
	ODArbitrator* arbitrator = session->GetArbitrator(ev);

	ODToken         clipboardFocus = session->Tokenize(ev, kODClipboardFocus);
	TempODFrame clipOwner = arbitrator->AcquireFocusOwner(ev, clipboardFocus);

	if ( (clipOwner == selectionFrame) || arbitrator->RequestFocus(ev, clipboardFocus, selectionFrame) )
	{
		ODClipboard* clipboard = session->GetClipboard(ev);
		TRY{
			ODStorageUnit* su = clipboard->GetContentStorageUnit(ev);
#ifndef _PLATFORM_WIN32_
			ODDraft* draft = su->GetDraft(ev);
			draft->Externalize(ev);
#endif
		}CATCH_ALL{
			WARNMSG("Error %d externalizing clipboard",ErrorCode());
			// If this failed the Clipboard is probably hosed now.
			clipboard->DiscardClipboard(ev);
		}ENDTRY

		if ( clipOwner != selectionFrame )
			arbitrator->RelinquishFocus(ev, clipboardFocus, selectionFrame);
	}
}
#endif
