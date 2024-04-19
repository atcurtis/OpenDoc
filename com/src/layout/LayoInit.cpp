/* @(#)Z 1.2 com/src/layout/LayoInit.cpp, odlayout, od96os2, odos29646d 96/11/15 15:25:31 (96/07/15 18:32:30) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odlayout
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
	File:		LayoInit.cp

	Contains:	Init routines for the Layout library

	Owned by:	Joshua Susser

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <8>	 8/29/95	jpa		Rename 'using' variable; it's a reserved
									word in CW7.
		 <7>	 8/11/95	jpa		Added LayoutInit for single-lib build &
									renumbered resource [1276412]
		 <6>	 5/26/95	RR		#1251403: Multithreading naming support
		 <5>	 1/12/95	jpa		Don't use obsolete Toolbox names [1211211].
									Don't export cfm init [1210936].
		 <4>	10/24/94	jpa		Fixed use of resources in CFMInit.
									[1186124]
		 <3>	 9/29/94	RA		1189812: Mods for 68K build.
		 <2>	 8/24/94	JBS		1181909: new active border appearance
		 <1>	 6/29/94	JBS		first checked in
		 <0>	 6/27/94	JBS		copied from CoreInit.cpp

	To Do:
	In Progress:
		
*/

#ifndef __CODEFRAGMENTS__
#include <CodeFragments.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __RESOURCES__
#include <Resources.h>
#endif

#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif

#ifndef __USERSRCM__
#include <UseRsrcM.h>
#endif

#ifndef _RSRCBASE_
#include <RsrcBase.h>
#endif


//==============================================================================
// Constants
//==============================================================================

const ODSShort kBorderPatternID = kODLayoutRsrcBase + 0;

//==============================================================================
// Global variables
//==============================================================================

Pattern borderPattern;

//==============================================================================
// Functions
//==============================================================================

OSErr LayoutInit( );

extern "C" pascal OSErr LayoutCFMInit( CFragInitBlockPtr );


OSErr LayoutInit( )
{
	CUsingLibraryResources u;
	
	PatHandle patRsrc = GetPattern(kBorderPatternID);
	OSErr err = ResError();
	if ( !patRsrc ) return err ?err :resNotFound;
	
	borderPattern = **patRsrc;
	
	ReleaseResource((Handle)patRsrc);

	return noErr;
}


pascal OSErr LayoutCFMInit (CFragInitBlockPtr initBlkPtr)
{
	OSErr err;
	err = InitLibraryResources(initBlkPtr);
	if ( err ) return err;
	return LayoutInit();
}

