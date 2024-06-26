/* @(#)Z 1.8 com/src/ui/FocusOwn.cpp, odui, od96os2, odos29646d 96/11/15 15:28:04 (96/07/15 18:26:41) */
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
	File:           FocusOwn.cpp

	Contains:       Implementation of ODFocusOwnerIterator

	Owned by:       Richard Rodseth

	Copyright:      � 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <5>     8/15/95        RR              #1276680 Removed redundant fields, and
									uninit overrides.
		 <4>      8/3/95        RR              #1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <3>     5/26/95        RR              #1251403: Multithreading naming support
		 <2>      4/7/95        RR              #1216618 Added SOM_TRY etc.
		 <1>     9/26/94        RR              first checked in

	To Do:
*/


/*
 *  This file was generated by the SOM Compiler.
 *  Generated using:
 *      SOM Emitter emitxtm.dll: somc/smmain.c
 */

#define ODFocusOwnerIterator_Class_Source
#define VARIABLE_MACROS
#include <FocusOwn.xih>

#ifndef SOM_Module_OpenDoc_Foci_defined
#include "Foci.xh"
#endif



#include "ODDebug.h"


SOM_Scope void  SOMLINK ODFocusOwnerIteratorInitFocusOwnerIterator(ODFocusOwnerIterator *somSelf, Environment *ev,
		ODTypeToken focus,
		ODFocusModule* focusModule)
{
    //ODFocusOwnerIteratorData *somThis = ODFocusOwnerIteratorGetData(somSelf);
    ODFocusOwnerIteratorMethodDebug("ODFocusOwnerIterator","ODFocusOwnerIteratorInitFocusOwnerIterator");

    SOM_TRY
			
		somSelf->InitObject(ev);
		
	SOM_CATCH_ALL
	SOM_ENDTRY
}

SOM_Scope ODFrame*  SOMLINK ODFocusOwnerIteratorFirst(ODFocusOwnerIterator *somSelf, Environment *ev)
{
    //ODFocusOwnerIteratorData *somThis = ODFocusOwnerIteratorGetData(somSelf);
    ODFocusOwnerIteratorMethodDebug("ODFocusOwnerIterator","ODFocusOwnerIteratorFirst");

	WARNMSG(WARN_INDEX(AMSG_520),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODNULL;
}

SOM_Scope ODFrame*  SOMLINK ODFocusOwnerIteratorNext(ODFocusOwnerIterator *somSelf, Environment *ev)
{
    //ODFocusOwnerIteratorData *somThis = ODFocusOwnerIteratorGetData(somSelf);
    ODFocusOwnerIteratorMethodDebug("ODFocusOwnerIterator","ODFocusOwnerIteratorNext");

	WARNMSG(WARN_INDEX(AMSG_520),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODNULL;
}

SOM_Scope ODBoolean  SOMLINK ODFocusOwnerIteratorIsNotComplete(ODFocusOwnerIterator *somSelf, Environment *ev)
{
    //ODFocusOwnerIteratorData *somThis = ODFocusOwnerIteratorGetData(somSelf);
    ODFocusOwnerIteratorMethodDebug("ODFocusOwnerIterator","ODFocusOwnerIteratorIsNotComplete");

	WARNMSG(WARN_INDEX(AMSG_520),"A subclass should have overridden this method!");
	ODSetSOMException(ev,kODErrSubClassResponsibility, "SubClass Responsibility");
	return kODFalse;
}
