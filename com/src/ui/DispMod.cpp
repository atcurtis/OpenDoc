/* @(#)Z 1.2 com/src/ui/DispMod.cpp, odui, od96os2, odos29646d 96/11/15 15:28:01 (96/07/15 18:26:02) */
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
	File:		DispMod.cpp

	Contains:	Implementation of ODDispatchModule

	Owned by:	Richard Rodseth

	Copyright:	� 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	 8/15/95	RR		#1276680 Removed redundant fields, and
									uninit and Purgeoverrides.
		 <5>	  8/3/95	RR		#1257260: Collapse B classes. Remove
									somInit methods. Don't call IsInitialized
									or SubclassResponsibility
		 <4>	 6/15/95	RR		#1254828/1255536 Added eventInfo to
									Dispatch
		 <3>	 5/26/95	RR		#1251403: Multithreading naming support
		 <2>	 4/13/95	RR		# 1216618 Added SOM_TRY
		 <1>	 9/26/94	RR		first checked in

	To Do:
*/


/*
 *  This file was generated by the SOM Compiler.
 *  Generated using: 
 *      SOM Emitter emitxtm.dll: somc/smmain.c
 */

#define ODDispatchModule_Class_Source
#define VARIABLE_MACROS
#include <DispMod.xih>

SOM_Scope void  SOMLINK ODDispatchModuleInitDispatchModule(ODDispatchModule *somSelf, Environment *ev,
		ODSession* session)
{
    //ODDispatchModuleData *somThis = ODDispatchModuleGetData(somSelf);
    ODDispatchModuleMethodDebug("ODDispatchModule","ODDispatchModuleInitDispatchModule");
		
	SOM_TRY
		
		somSelf->InitObject(ev);
		
	SOM_CATCH_ALL
	SOM_ENDTRY
}

SOM_Scope ODBoolean  SOMLINK ODDispatchModuleDispatch(ODDispatchModule *somSelf, Environment *ev,
		ODEventData* event,
		ODEventInfo* eventInfo)
{
    ODDispatchModuleMethodDebug("ODDispatchModule","ODDispatchModuleDispatch");

	return kODFalse;
}

