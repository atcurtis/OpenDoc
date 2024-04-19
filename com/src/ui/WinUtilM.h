/* @(#)Z 1.7 com/src/ui/WinUtilM.h, odui, od96os2, odos29646d 96/11/15 15:29:31 (96/10/29 09:34:11) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
 *
 *   CLASSES:   WindowLink
 *		WindowListIterator
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
/*  <OS2>  02/20/96 jrb            Port DR4                         */
/*  <OS2>  08/23/95 aml     133968 Port DR3                         */
/*                                                                  */
/********************************************************************/
/*
        File:           WinUtilM.h

        Contains:       Macintosh window utility functions and classes

        Owned by:       Richard Rodseth

        Copyright:      © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                 <5>     6/28/95        RR              1242642 BB Mostly ref counting. Added
                                                                        WindowLink::ShouldRemove
                 <4>     6/22/95        RR              #1245283 Undoable frame deletion
                                                                        #1209427 Changed private api between
                                                                        iterator and iteratee. Allow deletion while
                                                                        iterating. Added fRemove flag to WindowLink
                 <3>     5/25/95        jpa             List.h --> LinkList.h. Removed
                                                                        GetNextWindow, which is now in Toolbox
                                                                        <Windows.h> [1253324, 1241078]
                 <2>     4/14/95        T‚              With RR & CG: #1194507 DR/BB:title bar of a
                                                                        draft window doesn't reveal it is a draft
                                                                        or which draft it is.  Added
                                                                        GetDraftNumFromDraft.
                 <1>     6/16/94        RR              first checked in

        To Do:
        In Progress:

*/

#ifndef _WINUTILM_
#define _WINUTILM_

#ifndef _LINKLIST_
#include <LinkList.h>
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

//==============================================================================
// Classes used by this interface
//==============================================================================

class ODWindow;

//==============================================================================
// Function Prototypes
//==============================================================================

#ifdef _PLATFORM_MACINTOSH_
WindowPtr GetWindowList();
void SetWindowList(WindowPtr theWindow);
#endif
#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
// Function to return the next window in the Z-order relative to the input
// window
WindowPtr NextWindow(WindowPtr theWindow);
#endif
#ifdef _PLATFORM_UNIX_
// Function to return the next window in the Z-order relative to the input
// window
WindowPtr NextWindow(Window *windows,
                     unsigned int num_windows,
                     ODPlatformWindow theWindow);
#endif

#ifdef _PLATFORM_MACINTOSH_
void SetNextWindow(WindowPtr theWindow, WindowPtr nextWindow);
#endif
#if defined (_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
// Function to return the topmost window on the desktop
WindowPtr FrontWindow();
#endif
#ifdef _PLATFORM_UNIX_
// Function to return the topmost window on the desktop
WindowPtr FrontWindow(Screen *screen, Window **windows, unsigned int *num_windows);
#endif

ODBoolean GetWindowHilite(WindowPtr theWindow);
#ifdef _PLATFORM_MACINTOSH_
void SetWindowHilite(WindowPtr theWindow, ODBoolean windowHilite);
RgnHandle GetStructureRegion(WindowPtr theWindow);
RgnHandle GetContentRegion(WindowPtr theWindow);
#endif
#ifdef _PLATFORM_UNIX_
ODBoolean       GetWindowVisible(Display *display, WindowPtr theWindow);
#else
ODBoolean       GetWindowVisible(WindowPtr theWindow);
#endif
ODULong GetDraftNumFromDraft(Environment* ev, ODDraft* draft);
//=====================================================================================
// WindowLink
//=====================================================================================

class WindowLink : public Link
{
        public:
                WindowLink(ODID windowID, ODWindow* window);
                ~WindowLink() {}

                ODBoolean ShouldRemove();

                ODID            fID;
                ODWindow* fWindow;
                ODBoolean fRemove;
};

//=====================================================================================
// WindowListIterator - iterates over the Window Manager window list. Iterating from
// back to front using Last/Previous obviously results in several scans of the list,
// as it's a singly-linked list
//=====================================================================================


class WindowListIterator
{
public:
        WindowListIterator();
        ~WindowListIterator();

        ODBoolean IsNotComplete();

#ifdef _PLATFORM_UNIX_
        WindowPtr First(Screen *screen);
#else
        WindowPtr First();
#endif

        WindowPtr Next();

#ifdef _PLATFORM_UNIX_
        WindowPtr Last(Screen *screen);
#else
        WindowPtr Last();
#endif

        WindowPtr Previous();

private:
        WindowPtr fCurrentWindow;
#ifdef _PLATFORM_UNIX_
   Screen *screen;
   Window *windows;
   unsigned int num_windows;
#endif
};


#endif //_WINUTILM_
