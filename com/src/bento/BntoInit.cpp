/* @(#)Z 1.2 com/src/bento/BntoInit.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:07 (96/07/15 18:16:27) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odstorage
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
	File:		BntoInit.cpp

	Contains:	Init routines for the Bento shared library

	Owned by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<11>	 9/29/95	TJ		Made Changes for MAC SOM
		<10>	 5/26/95	VL		1251403: Multithreading naming support.
		 <9>	 4/29/95	T‚		1244775 Warning when compiling BntoInit.cpp
		 <8>	 2/10/95	VL		1205627: Added ODBentoUpdateContainer.
		 <7>	 1/12/95	jpa		Don't use obsolete Toolbox names [1211211]
		 <6>	11/17/94	CG		#1181489: Uncommented implementation of
									BentoSOMInit.
		 <5>	 9/29/94	RA		1189812: Mods for 68K build.
		 <4>	 8/19/94	NP		1181622: Ownership fix.
		 <3>	 8/17/94	CG		#1181489: Made ready for Container Suite
									instantiation by name.
		 <2>	 7/19/94	VL		Added InitLibraryResources and changed
									DebugStr to somPrintf.
		 <1>	 6/23/94	NP		first checked in
		 <1>	  5/5/94	NP		first checked in

	To Do:
*/


#ifndef __USERSRCM__
#include "UseRsrcM.h"
#endif

#ifndef SOM_ODFileContainer_xh
#include <FileCtr.xh>
#endif

#ifndef SOM_ODEmbeddedContainer_xh
#include <EmbedCtr.xh>
#endif

#ifndef SOM_ODMemContainer_xh
#include <MemCtr.xh>
#endif

#ifndef SOM_ODUpdateContainer_xh
#include <UpdatCtr.xh>
#endif

void BentoInit ( );
                                       
void BentoInit ( )
{
	//somPrintf("In BentoSOMInit!");

	somNewClass ( ODFileContainer );
	somNewClass ( ODEmbeddedContainer );
	somNewClass ( ODMemContainer );
	somNewClass ( ODUpdateContainer );
	//ODFileContainerNewClass(majorVersion, minorVersion);
	//ODEmbeddedContainerNewClass(majorVersion, minorVersion);
	//ODMemContainerNewClass(majorVersion, minorVersion);
	//ODUpdateContainerNewClass(majorVersion, minorVersion);

} /* SOMInitModule () */

extern "C" pascal OSErr BentoCFMInit( CFragInitBlockPtr );

pascal OSErr BentoCFMInit (CFragInitBlockPtr initBlkPtr)
{
	return InitLibraryResources(initBlkPtr);
}

