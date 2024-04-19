/* @(#)Z 1.2 com/src/binding/BndgInit.cpp, odconfig, od96os2, odos29646d 96/11/15 15:24:42 (96/07/15 18:07:22) */
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
	File:		BndgInit.cpp

	Contains:	Init routines for the Binding library

	Owned by:	Caia Grisar

	Copyright:	© 1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	 1/12/95	jpa		Don't use obsolete Toolbox names [1211211].
									Don't export cfm init [1210936].
		 <5>	 9/29/94	RA		1189812: Mods for 68K build.
		 <4>	 8/24/94	CG		#1181622: Updated file ownership.
		 <3>	 6/30/94	jpa		Added InitLibraryResources call.
		 <2>	 6/23/94	NP		Clean up.
		 <1>	 6/10/94	CG		first checked in

	To Do:
*/


#ifndef __USERSRCM__
#include "UseRsrcM.h"
#endif

extern "C" pascal OSErr BindingCFMInit (CFragInitBlockPtr);

pascal OSErr BindingCFMInit (CFragInitBlockPtr initBlkPtr)
{
	return InitLibraryResources(initBlkPtr);
}
