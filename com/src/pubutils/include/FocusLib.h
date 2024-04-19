/* @(#)Z 1.4 os2/src/pubutils/include/FocusLib.h, odpubutils, od96os2, odos29646d 96/11/15 15:50:43 (96/08/23 01:42:34) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odpubutils
 *
 *   CLASSES:   CFocus
 *		CFocusWindow
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

#ifdef _PLATFORM_OS2_
// We have an OS/2 platform specific version here.

/*
	File:		FocusLib.h

	Contains:	Library routines for focusing (setting up for drawing into a facet)

	Owned by:	Jens Alfke

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.
		 
		 
	THEORY OF OPERATION:
	
		FocusLib sets up the drawing environment for a QuickDraw-based part so that
		it can start drawing into a facet. It provides both a one-time change of
		focus (via the Focus call) and a way to set the focus but restore it later
		when the current scope exits (via the CFocus object.)
		
		Focus() and FocusWindow() are slightly different for offscreen facets. If the
		facet is in an offscreen canvas, Focus will set up to draw into that canvas;
		this is what one usually wants to do. On the other hand, FocusWindow will
		always focus onto an on-screen canvas, even for an off-screen facet. Use
		FocusWindow for things like rubber-banding where your drawing needs to show
		up immediately, even if the facet is offscreen.

		The CFocus object deserves an example:
		
		void DrawSomething( ODFacet *facet )
		{
			CFocus f(facet);		// Creates a CFocus object on the stack, which
									// changes the focus but stores the old state
			....drawing code...
		}
		
		Declaring the CFocus object calls its constructor, which sets the focus and
		stores the old state in the CFocus object itself (on the stack.) When its
		scope exists, the CFocus object's destructor is called, which restores the
		old focus.
		
		CFocus inherits from Destructo (see Except.h) so it will now be destructed
		properly if an exception is thrown while it's active. You no longer need to
		catch the exception and manually destruct the CFocus.
*/


#ifndef _FOCUSLIB_
#define _FOCUSLIB_

#ifndef _ODOS2_
#include <odos2.h>
#endif 

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifdef __cplusplus
	class ODCanvas;
	class ODShape;
	class ODFacet;
	struct Environment;
#else
	#ifndef som_h
	#include <som.h>
	#endif
	#ifndef SOM_ODCanvas_h
	#include <Canvas.h>
	#endif
	#ifndef SOM_ODShape_h
	#include <Shape.h>
	#endif
	#ifndef SOM_ODFacet_h
	#include <Facet.h>
	#endif
#endif


/*	FocusState stores the state data for QD focusing.
	C users should allocate one on the stack and call BeginFocus and
	EndFocus (q.v.) to do the focusing.
	C++ users should ignore FocusState and simply allocate a CFocus object
	(see below).
*/
struct FocusState
{
  HPS          fHPS;
  HWND         fHWND;
  HWND         fToWindow;
  HRGN         fClipRgn;
  ODFacet*     fFacet;
  ODBoolean    fDragPS;
  ODShape        *fclipShape;
  Environment* fEv;

#ifdef __cplusplus
  void    BeginFocus(ODFacet *facet, ODShape* invalShape, ODBoolean setClipRgn,
                    ODBoolean toWindow, ODBoolean lockedWindowUpdate, ODBoolean dragPS);
  void    EndFocus( );
#endif
};
typedef struct FocusState FocusState;

#ifdef __cplusplus

/*	CFocus is a class for C++ users. Just allocate one as a local variable:
	the constructor will focus, and the destructor (called when it goes out
	of scope or an exception is thrown) will unfocus.
	CFocusWindow is just like CFocus, but focuses to the window instead of
	the facet's canvas (if they're different.)
*/
class CFocus {
  public:
  CFocus(ODFacet *facet, ODShape* invalShape, HPS *theHPS );
  CFocus(ODFacet *facet, ODShape* invalShape, HPS *theHPS, HRGN* theClipRgn );
  virtual ~CFocus();

protected:
  FocusState f;
};

class CFocusWindow {
  public:
  enum TypePS {
    NormalPS,
    DragPS,
    LockedWindowUpdatePS
  };
    
  CFocusWindow(ODFacet *facet, ODShape* invalShape,
                            HPS *theHPS, HWND *theHWND = kODNULL, TypePS typePS = NormalPS);

  CFocusWindow(ODFacet *facet, ODShape* invalShape,
                HPS *theHPS, HWND *theHWND, TypePS typePS, HRGN *theClipRgn);
  virtual ~CFocusWindow();
  
  protected:
  FocusState f;
};
#endif


#ifdef __cplusplus
extern "C" {
#endif

// For C only:
void BeginFocus(FocusState* foc, ODFacet *facet, ODShape* invalShape, ODBoolean setClipRgn,
         ODBoolean toWindow, ODBoolean lockedWindowUpdate, ODBoolean dragPS);

void EndFocus( FocusState* );

#ifdef __cplusplus
}
#endif


#endif //_FOCUSLIB_
#endif // _PLATFORM_OS2_
