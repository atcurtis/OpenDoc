/* @(#)Z 1.6 com/src/bento/SURef.cpp, odstorage, od96os2, odos29646d 96/11/15 15:26:35 (96/10/29 09:13:35) */
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
	File:		SURef.cpp

	Contains:	Implementation SURefKeeper class.

	Owned by:	Vincent Lo

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	10/24/95	jpa		1293441: DMC: Bento memory reserve.
		 <5>	 8/12/95	T‚		1276806 Optimization: use kODFalse instead
									of kODTrue in comparisons
		 <4>	 5/26/95	VL		1251403: Multithreading naming support.
		 <3>	 4/27/95	EL		1241327: The previous checkin destroy the
									ref and write that out.
		 <2>	 3/29/95	DM		make standard format little endian
		 <1>	 1/26/95	VL		first checked in
	To Do:
	In Progress:
		
*/

#ifndef _SUREF_
#include "SURef.h"
#endif

#ifndef _FLIPEND_
#include "FlipEnd.h"
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef SOM_Module_OpenDoc_StdProps_defined
#include <StdProps.xh>
#endif

#ifndef SOM_Module_OpenDoc_StdTypes_defined
#include <StdTypes.xh>
#endif

#ifndef __CM_API__
#include "CMAPI.h"
#endif

#ifndef _EXCEPT_
#include <Except.h>
#endif

#ifndef _BENTODEF_
#include "BentoDef.h"
#endif

#ifndef SOM_CMStorageUnit_xh
#include <CMSU.xh>
#endif

#ifndef _SESSHDR_
#include "SessHdr.h"
#endif

//==============================================================================
// Constants
//==============================================================================

#define kPlatformRefLowBitsMask 0x7FFFFFFF
#define kPlatformRefHighBitMask 0x80000000

#ifdef _PLATFORM_BIG_ENDIAN_

#define kStdRefLowBitsMask 0xFFFFFF7F
#define kStdRefHighBitMask 0x00000080

#else

#define kStdRefLowBitsMask 0x7FFFFFFF
#define kStdRefHighBitMask 0x80000000

#endif /* _PLATFORM_BIG_ENDIAN_ */

//==============================================================================
// Function Prototype
//==============================================================================

ODULong GetULRefValueFromSURef(ODStorageUnitRef ref);


//------------------------------------------------------------------------------
// GetULRefValueFromSURef
//------------------------------------------------------------------------------

ODULong GetULRefValueFromSURef(ODStorageUnitRef ref)
{
	ODULong	longValue = ConvertODULongFromStd(*((ODULong*) ref));
	return longValue & kPlatformRefLowBitsMask;
}

//==============================================================================
// SURefKeeper
//==============================================================================

//------------------------------------------------------------------------------
// SURefKeeper::SURefKeeper
//------------------------------------------------------------------------------

SURefKeeper::SURefKeeper(CMStorageUnit* su)
{
	fSU = su;
	fCurrentSURef = 0;
	fDirty = kODFalse;
}

//------------------------------------------------------------------------------
// SURefKeeper::~SURefKeeper
//------------------------------------------------------------------------------

SURefKeeper::~SURefKeeper()
{
}

//------------------------------------------------------------------------------
// SURefKeeper::Internalize
//------------------------------------------------------------------------------

void SURefKeeper::Internalize()
{
        Environment* ev = somGetGlobalEnvironment();
	CMContainer container = fSU->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);
	
	CMObject	object = fSU->GetObject(ev);
	
	if (fCurrentSURef == 0) {
		CMProperty property = CMRegisterProperty(container, kODPropCurrentSURef);
		if (property == kODNULL)
			THROW(kODErrBentoInvalidProperty);
		
		CMType type = CMRegisterType(container, kODULong);
		if (type == kODNULL)
			THROW(kODErrBentoInvalidType);

		CMValue value = CMUseValue(object, property, type);
		if (value != kODNULL) {
			ODULong stdRef;
			CMReadValueData(value, &stdRef, 0, sizeof(ODStorageUnitRef));
			CMReleaseValue(value);
			fCurrentSURef = ConvertODULongFromStd(stdRef);
		}
		else {
			ODULong stdRef = ConvertODULongToStd(fCurrentSURef);
			value = CMNewValue(object, property, type);
			CMWriteValueData(value, &stdRef, 0, sizeof(ODStorageUnitRef));
		}
	}
	ODSessionRestoreCMAllocReserve(container);
}

//------------------------------------------------------------------------------
// SURefKeeper::Externalize
//------------------------------------------------------------------------------

void SURefKeeper::Externalize()
{
	Environment* ev = somGetGlobalEnvironment();
	CMContainer container = fSU->GetCMContainer(ev);
	ODSessionMustHaveCMAllocReserve(container);

	CMObject	object = fSU->GetObject(ev);

	if (fDirty != kODFalse) {
		CMProperty property = CMRegisterProperty(container, kODPropCurrentSURef);
		if (property == kODNULL)
			THROW(kODErrBentoInvalidProperty);
		
		CMType type = CMRegisterType(container, kODULong);
		if (type == kODNULL)
			THROW(kODErrBentoInvalidType);

		ODULong stdRef = ConvertODULongToStd(fCurrentSURef);
		
		CMValue value = CMUseValue(object, property, type);
		if (value != kODNULL) {
			CMWriteValueData(value, &stdRef, 0, sizeof(ODStorageUnitRef));
			CMReleaseValue(value);
		}
		else {
			value = CMNewValue(object, property, type);
			CMWriteValueData(value, &stdRef, 0, sizeof(ODStorageUnitRef));
		}
		
		fDirty = kODFalse;
	}
	ODSessionRestoreCMAllocReserve(container);
}
	
//------------------------------------------------------------------------------
// SURefKeeper::GetNextSURef
//------------------------------------------------------------------------------

void SURefKeeper::GetNextSURef(ODStorageUnitRef ref, ODBoolean strong)
{
	Internalize();
	fCurrentSURef++;
	fDirty = kODTrue;
	
	ODULong ulRef = fCurrentSURef;
	if (strong != kODFalse)
		ulRef = ulRef & kPlatformRefLowBitsMask;
	else
		ulRef = ulRef | kPlatformRefHighBitMask;
	
	*((ODULong*) ref) = ConvertODULongToStd(ulRef);
}

//------------------------------------------------------------------------------
// SURefKeeper::Reset
//------------------------------------------------------------------------------

void SURefKeeper::Reset(ODStorageUnitRef ref)
{
	ODULong	inRef = GetULRefValueFromSURef(ref);

	if (inRef > fCurrentSURef) {
		fCurrentSURef = inRef;
		fDirty = kODTrue;
	}
}
	
//------------------------------------------------------------------------------
// SURefKeeper::IsStrongSURef
//------------------------------------------------------------------------------

ODBoolean SURefKeeper::IsStrongSURef(ODStorageUnitRef ref)
{
	return (*((ODULong*) ref) & kStdRefHighBitMask ? kODFalse : kODTrue);
}

//------------------------------------------------------------------------------
// SURefKeeper::IsWeakSURef
//------------------------------------------------------------------------------

ODBoolean SURefKeeper::IsWeakSURef(ODStorageUnitRef ref)
{
	return (*((ODULong*) ref) & kStdRefHighBitMask ? kODTrue : kODFalse);
}

//------------------------------------------------------------------------------
// SURefKeeper::InvalidateSURef
//------------------------------------------------------------------------------

void SURefKeeper::InvalidateSURef(ODStorageUnitRef ref)
{
	*((ODULong*) ref) = 0;
}

//------------------------------------------------------------------------------
// SURefKeeper::IsValidSURef
//------------------------------------------------------------------------------

ODBoolean SURefKeeper::IsValidSURef(ODStorageUnitRef ref)
{
	return (*((ODULong*) ref) == 0 ? kODFalse : kODTrue);
}
