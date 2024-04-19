/* @(#)Z 1.14 com/src/ui/WinUtilM.cpp, odui, od96os2, odos29646d 96/11/15 15:28:27 (96/10/29 09:34:06) */
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
        File:           WinUtilM.cpp

        Contains:       Utilities to manipulate Mac windows

        Owned by:       Richard Rodseth

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

  IBM Change History (most recent first)

     <2>   02/20/96  jrb         - DR4 port of Apple code
     <1>   08/24/95  pfe 133968  - DR3 port of Apple code

        Change History (most recent first):

                 <8>      9/8/95        T‚              1281096 FB2:Many constants in ODTypesB
                                                                        without kOD prefix!
                 <7>     6/28/95        RR              1242642 BB Mostly ref counting. Add
                                                                        WindowLink::ShouldRemove
                 <6>     6/22/95        RR              #1245283 Undoable frame deletion
                                                                        #1209427 Changed private api between
                                                                        iterator and iteratee. Allow deletion while
                                                                        iterating. Added fRemove flag to WindowLink
                 <5>     5/26/95        RR              #1251403: Multithreading naming support
                 <4>     5/25/95        jpa             Removed GetNextWindow, which is now in
                                                                        Toolbox <Windows.h> [1241078, 1253324]
                 <3>     4/14/95        T‚              With RR & CG: #1194507 DR/BB:title bar of a
                                                                        draft window doesn't reveal it is a draft
                                                                        or which draft it is.  Add
                                                                        AcquireDraftNumFromDraft
                 <2>     9/29/94        RA              1189812: Mods for 68K build.
                 <1>     6/16/94        RR              first checked in

        To Do:
        In Progress:

*/

#ifdef _PLATFORM_OS2_
#include <ODos2.h>
#endif

#ifdef _PLATFORM_WIN32_
#ifndef _WINDOWS_
#include <windows.h>
#endif
#endif

#ifdef _PLATFORM_UNIX_
#include <Xm/XmAll.h>
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef _WINUTILM_
#include "WinUtilM.h"
#endif
#endif

#ifdef _PLATFORM_MACINTOSH_
#ifndef __LOWMEM__
#include <LowMem.h> // For WindowList global
#endif
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

#ifndef SOM_ODDocument_xh
#include <Document.xh>
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
#ifndef _WINUTILM_
#include "WinUtilM.h"
#endif

#ifndef _ODUTILS_
#include "ODUtils.h"
#endif

#ifndef _TEMPOBJ_
#include <TempObj.h>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_ODStorageUnit_xh
#include <StorageU.xh>
#endif

#ifndef _STORUTIL
#include <StorUtil.h>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef _STDTYPIO_
#include <StdTypIO.h>
#endif

#endif // _PLATFORM_OS2_ || _PLATFORM_WIN32_ || _PLATFORM_UNIX_

// New Window API uses a new "WindowRef" type as an opaque window type. So that we can compile
// with or without the new Windows.h, define WindowRef if it's not already defined:      --jpa
#ifndef STRICT_WINDOWS
#define WindowRef WindowPeek
#endif


#ifdef _PLATFORM_MACINTOSH_
WindowPtr GetWindowList()
{
        return (WindowPtr)LMGetWindowList();
}

void SetWindowList(WindowPtr theWindow)
{
        LMSetWindowList((WindowRef)theWindow); /* Adkins -- changed from WindowPeek to WindowRef */
}

void SetNextWindow(WindowPtr theWindow, WindowPtr nextWindow)
{
        ((WindowRecord*) theWindow)->nextWindow = (WindowPeek) nextWindow;
}
#endif

#ifdef _PLATFORM_OS2_
// Function to get next window in the Z-order
WindowPtr NextWindow(ODPlatformWindow theWindow)
{
        return (ODPlatformWindow) WinQueryWindow(theWindow,QW_NEXT);
}
#endif

#ifdef _PLATFORM_WIN32_
// Function to get next window in the Z-order.  Note that I couldn't
// use the name GetNextWindow as in the Mac as GetNextWindow is a
// macro in windows
WindowPtr NextWindow(ODPlatformWindow theWindow)
{
        return (ODPlatformWindow) GetWindow(theWindow, GW_HWNDNEXT);
}
#endif

#ifdef _PLATFORM_UNIX_
WindowPtr NextWindow(Window *windows,
                     unsigned int num_windows,
                     ODPlatformWindow theWindow)
{
    Window NextWindow = 0;

    // Find this window in the window array
    for (unsigned int index = 0;
         ((index < num_windows) && (theWindow != windows[index]));
         index++)
    {
        // do nothing
    }

    // If windows are in back of this window - UNIX windows have bottom at
    // index 0
    if (index > 0)
    {
        // We found the next window
        NextWindow = windows[index - 1];
    }
    else
    {
        // We let NextWindow remain at zero since we are out of windows
    }

    return NextWindow;
}
#endif

#ifdef _PLATFORM_UNIX_
ODBoolean GetWindowVisible(Display *display, WindowPtr theWindow)
{
    // Get the window's attributes
    XSync(display, False);
    XWindowAttributes attributes;
    XGetWindowAttributes(display, theWindow, &attributes);

    // Return whether the window is visible
    return (attributes.map_state == IsViewable ? kODTrue : kODFalse);
}
#else
ODBoolean GetWindowVisible(WindowPtr theWindow)
{
#ifdef _PLATFORM_MACINTOSH_
        return (((WindowRecord*) theWindow)->visible ? kODTrue : kODFalse);
#endif
#ifdef _PLATFORM_OS2_ // CED
  // Return whether the window is visible
  return WinIsWindowVisible(theWindow);
#endif // _PLATFORM_OS2_
#ifdef _PLATFORM_WIN32_
  // Return whether the window is visible
  return IsWindowVisible(theWindow);
#endif
}
#endif

ODBoolean GetWindowHilite(WindowPtr theWindow)
{
#ifdef _PLATFORM_MACINTOSH_
        return (((WindowRecord*) theWindow)->hilited ? kODTrue : kODFalse);
#endif
#ifdef _PLATFORM_OS2_
    // Return whether the window is visible
    return WinIsWindowVisible(theWindow);
#endif // _PLATFORM_OS2_
#ifdef _PLATFORM_WIN32_
    // Return whether the window is visible
    return IsWindowVisible(theWindow);
#endif
#ifdef _PLATFORM_UNIX_
    // This is never used
    return kODFalse;
#endif
}

#ifdef _PLATFORM_MACINTOSH_
void SetWindowHilite(WindowPtr theWindow, ODBoolean windowHilite)
{
        ((WindowRecord*) theWindow)->hilited = windowHilite;
}

RgnHandle GetStructureRegion(WindowPtr theWindow)
{
        return ((WindowRecord*) theWindow)->strucRgn;
}

RgnHandle GetContentRegion(WindowPtr theWindow)
{
        return ((WindowRecord*) theWindow)->contRgn;
}
#endif // _PLATFORM_MACINTOSH_

#ifdef _PLATFORM_OS2_
// Function to get the topmost window of the desktop
WindowPtr FrontWindow()
{
   // Return the topmost desktop window
   return WinQueryWindow(HWND_DESKTOP, QW_TOP);
}
#endif

#ifdef _PLATFORM_WIN32_
// Function to get the topmost window of the desktop
WindowPtr FrontWindow()
{
   // Return the topmost desktop window
   return GetTopWindow(NULL);
}
#endif

#ifdef _PLATFORM_UNIX_
WindowPtr FrontWindow(Screen *screen, Window **windows, unsigned int *num_windows)
{
    // Get the root window
    Window root = RootWindowOfScreen(screen);

    Window root2, parent;
    // Get the tree for the root
    XQueryTree(DisplayOfScreen(screen),
               root,
               &root2,
               &parent,
               windows,
               num_windows);

    // Return the last window in the array - this is the topmost window in UNIX
    return ((*windows)[*num_windows - 1]);
}
#endif

ODULong GetDraftNumFromDraft(Environment* ev, ODDraft* draft)
{
  // Initialize draft number to 0.  If the draft doesn't have the DraftNumber
  // property, zero is the value we want
  ODULong draftNumber = 0;

#ifdef _PLATFORM_MACINTOSH_
  ODDocument* document = draft->GetDocument(ev);

  if (document->Exists(ev, 0, draft, kODPosFirstAbove))
  {
     ODDraft* aDraft = document->AcquireBaseDraft(ev, kODDPReadOnly);
     draftNumber = 1;
     while (!ODObjectsAreEqual(ev, aDraft, draft))
     {
       ++draftNumber;
       aDraft = document->AcquireDraft(ev, kODDPReadOnly, 0, aDraft,
                                                                kODPosFirstAbove, kODTrue);
     }
         ODReleaseObject(ev, aDraft);
  }
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_UNIX_)
  // Retrieve draft's properties
  TempODStorageUnit DraftProperties = draft->AcquireDraftProperties(ev);

  // If the draft number property exists - it won't exist if the user has never
  // opened draft
  if (DraftProperties->Exists(ev,
                              kODPropDraftNumber,
                              kODULong,
                              0))
  {
      // Get draft number - this routine will also take into account any
      // endian-ness of the long
      draftNumber = ODGetULongProp(ev, DraftProperties, kODPropDraftNumber, kODULong);
  }
#endif

  // Return the draft number to the caller
  return draftNumber;
}

//=====================================================================================
// WindowLink
//=====================================================================================

//-------------------------------------------------------------------------------------
// WindowLink::WindowLink
//
// Description
//-------------------------------------------------------------------------------------

WindowLink::WindowLink(ODID windowID, ODWindow* window)
{
        fID = windowID;
        fWindow = window;
        fRemove = kODFalse;
}

ODBoolean WindowLink::ShouldRemove()
{
        if (fRemove)
                return kODTrue; // Separate line for debugging purposes
        else
                return kODFalse;
}

//=====================================================================================
// WindowListIterator - iterates over the Window Manager window list. Iterating from
// back to front using Last/Previous obviously results in several scans of the list,
// as it's a singly-linked list
//=====================================================================================



WindowListIterator::WindowListIterator()
{
        fCurrentWindow = kODNULL;

#ifdef _PLATFORM_UNIX_
        windows     = NULL;
        num_windows = 0;
#endif
}

WindowListIterator::~WindowListIterator()
{
#ifdef _PLATFORM_UNIX_
        if (windows)
        {
            XFree(windows);
        }
#endif
}

#ifdef _PLATFORM_UNIX_
WindowPtr WindowListIterator::First(Screen *screen)
{
        fCurrentWindow = FrontWindow(screen, &windows, &num_windows);
        return fCurrentWindow;
}
#else
WindowPtr WindowListIterator::First()
{
        fCurrentWindow = FrontWindow();
        return fCurrentWindow;
}
#endif

WindowPtr WindowListIterator::Next()
{
        if (fCurrentWindow)
#ifdef _PLATFORM_MACINTOSH_
                fCurrentWindow = (WindowPtr)(((WindowPeek)fCurrentWindow)->nextWindow);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
        // Get the next window in the Z-order
        fCurrentWindow = NextWindow(fCurrentWindow);
#endif // _PLATFORM_OS2_ || _PLATFORM_WIN32_
#ifdef _PLATFORM_UNIX_
        // Get the next window in the Z-order
        fCurrentWindow = NextWindow(windows, num_windows, fCurrentWindow);
#endif // _PLATFORM_UNIX_
        return fCurrentWindow;
}

#ifdef _PLATFORM_UNIX_
WindowPtr WindowListIterator::Last(Screen *screen)
{
    // Get the front window - this sets everything up
    fCurrentWindow = First(screen);

    // Set up current window to be the first one in the array.
    // AIX windows are stacked with bottom at index 0
    fCurrentWindow = windows[0];

    return fCurrentWindow;
}
#else
WindowPtr WindowListIterator::Last()
{
#ifdef _PLATFORM_MACINTOSH_
        fCurrentWindow = FrontWindow();
        if (fCurrentWindow)
        {
                WindowPtr nextWindow = (WindowPtr)(((WindowPeek)fCurrentWindow)->nextWindow);
                while (nextWindow)
                {
                        fCurrentWindow = nextWindow;
                        nextWindow = (WindowPtr)(((WindowPeek)fCurrentWindow)->nextWindow);
                }
        }
#endif
#ifdef _PLATFORM_OS2_ // CED
    // Get the last window in the Z-order
    fCurrentWindow = WinQueryWindow(HWND_DESKTOP, QW_BOTTOM);
#endif
#ifdef _PLATFORM_WIN32_
    // Get the last window in the Z-order
        fCurrentWindow = GetWindow(FrontWindow(), GW_HWNDLAST);
#endif
        return fCurrentWindow;
}
#endif

WindowPtr WindowListIterator::Previous()
{
#ifdef _PLATFORM_MACINTOSH_
        WindowPtr savedCurrent = fCurrentWindow;

        fCurrentWindow = FrontWindow();

        while (fCurrentWindow && ( (WindowPtr) (((WindowPeek)fCurrentWindow)->nextWindow) != savedCurrent))
        {
                        fCurrentWindow = (WindowPtr) (((WindowPeek) fCurrentWindow)->nextWindow);
        }
#endif
#ifdef _PLATFORM_OS2_ // CED
    // If there is currently a window
    if (fCurrentWindow)
    {
        // Find the previous window in the Z-order
        fCurrentWindow = WinQueryWindow(fCurrentWindow, QW_PREV);
    }
#endif
#ifdef _PLATFORM_WIN32_
    // If there is currently a window
    if (fCurrentWindow)
    {
        // Find the previous window in the Z-order
        fCurrentWindow = GetWindow(fCurrentWindow, GW_HWNDPREV);
    }
#endif
#ifdef _PLATFORM_UNIX_
    // If there is currently a window
    if (fCurrentWindow)
    {
        // Find the previous window in the Z-order
        Window PrevWindow = 0;

        // Find the current window in the window array
        for (unsigned int index = 0;
             ((index < num_windows) && (fCurrentWindow != windows[index]));
             index++)
        {
            // do nothing
        }

        // If windows are in front of this window - UNIX windows have bottom at
        // index 0 and top as the last element in the array
        if (++index < num_windows)
        {
            // There is previous window
            PrevWindow = windows[index];
        }
        else
        {
            // We let PrevWindow remain at zero since we are out of windows
        }

        fCurrentWindow = PrevWindow;
    }
#endif

    return fCurrentWindow;
}

ODBoolean WindowListIterator::IsNotComplete()
{
        return (fCurrentWindow != kODNULL);
}



