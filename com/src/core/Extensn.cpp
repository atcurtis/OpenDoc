/* @(#)Z 1.5 com/src/core/Extensn.cpp, odcore, od96os2, odos29646d 96/11/15 15:24:45 (96/10/29 09:20:35) */
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
	File:		Extensn.cpp

	Contains:	Implementation of ODExtension class.

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<14>	 10/8/95	TJ		Fixes Recomended by Refball
		<13>	 8/16/95	NP		1275241: IDL Review. Remove unnecessary
									overrides.
		<12>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		<11>	 6/22/95	jpa		Call inherited Release method. [1261632]
		<10>	 6/20/95	JP		1251250: Made Release call ReleaseExtension
		 <9>	 4/28/95	eeh		1242417: raise error in CheckValid
		 <8>	 4/26/95	CG		1211082 BB: 5$ Bugs need to be evaluated
									and removed from Core
		 <7>	 4/14/95	T‚		#1239963 BB: InitExtension needs to call
									InitRefCntObject NOT InitObject
									 -- Note: this should allow ShellPlugIns to
									work.   Yeah!
		 <6>	 8/28/94	VL		1181298: Removed Release as ODExtension is
									a subclass of RefCntObject now.
		 <5>	 8/19/94	NP		1181622: Ownership fix.
		 <4>	 8/15/94	T‚		#1181162 Added BaseRemoved, IsValid &
									CheckValid
		 <3>	  7/8/94	NP		Fixed Initialization.
		 <2>	 6/24/94	CG		Initialization clean up.
		 <1>	 6/23/94	CG		first checked in
		 <0>	 6/22/94	SV		SOMverted
		 <2>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <3>	 1/14/94	NP		Init changes.
		 <2>	 4/28/93	NP		File name changes.
		 <1>	 4/27/93	NP		first checked in

	To Do:
	In Progress:
		
*/

#define ODExtension_Class_Source
#define VARIABLE_MACROS
#include <Extensn.xih>


#pragma segment ODExtension


//==============================================================================
// ODExtension
//==============================================================================

//------------------------------------------------------------------------------
// ODExtension: Initialize
//------------------------------------------------------------------------------


SOM_Scope void  SOMLINK ODExtensionInitExtension
		(ODExtension *somSelf, Environment *ev,
		ODObject* base)
{
    ODExtensionData *somThis = ODExtensionGetData(somSelf);
    ODExtensionMethodDebug("ODExtension","InitExtension");
	
	try {

	/* Moved from somInit. SOM itself sets fields to zero
	_fBase = kODNULL;
	*/
	somSelf->InitRefCntObject(ev);
	
	_fBase = base;
        }	
	catch (ODException _exception) {
            SetErrorCode(kODNoError);
        }
}

//------------------------------------------------------------------------------
// ODExtension: GetBase
//------------------------------------------------------------------------------

SOM_Scope ODObject*  SOMLINK ODExtensionGetBase
		(ODExtension *somSelf, Environment *ev)
{
    ODExtensionData *somThis = ODExtensionGetData(somSelf);
    ODExtensionMethodDebug("ODExtension","GetBase");

	return _fBase;
}

#if 0
SOM_Scope void  SOMLINK ODExtensionsomUninit(ODExtension *somSelf)
{
    ODExtensionData *somThis = ODExtensionGetData(somSelf);
    ODExtensionMethodDebug("ODExtension","somUninit");

    ODExtension_parent_ODRefCntObject_somUninit(somSelf);
}
#endif /* 0 */

SOM_Scope void  SOMLINK ODExtensionRelease
		(ODExtension *somSelf, Environment *ev)
{
    ODExtensionData *somThis = ODExtensionGetData(somSelf);
    ODExtensionMethodDebug("ODExtension","Release");

    ODExtension_parent_ODRefCntObject_Release(somSelf,ev);

	if (_fBase != kODNULL && somSelf->GetRefCount(ev) == 0)
		_fBase->ReleaseExtension(ev, somSelf);
}

SOM_Scope void  SOMLINK ODExtensionBaseRemoved
		(ODExtension *somSelf, Environment *ev)
{
    ODExtensionData *somThis = ODExtensionGetData(somSelf);
    ODExtensionMethodDebug("ODExtension","BaseRemoved");

	_fBase = kODNULL;
}

SOM_Scope ODBoolean  SOMLINK ODExtensionIsValid
		(ODExtension *somSelf, Environment *ev)
{
    ODExtensionData *somThis = ODExtensionGetData(somSelf);
    ODExtensionMethodDebug("ODExtension","BaseRemoved");

	return (_fBase != kODNULL);
}

SOM_Scope void  SOMLINK ODExtensionCheckValid
		(ODExtension *somSelf, Environment *ev)
{
    ODExtensionData *somThis = ODExtensionGetData(somSelf);
    ODExtensionMethodDebug("ODExtension","CheckValid");

	if (_fBase == kODNULL)
		ODSetSOMException( ev, kODErrInvalidExtension );
}

