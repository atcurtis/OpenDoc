/* @(#) 1.9 com/src/storage/LinkSpcB.cpp, oddataxfer, od96os2, odos29646d 10/31/96 07:56:29 [11/15/96 15:29:26] */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: oddataxfer
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
        File:           LinkSpecB.cpp

        Contains:       Implementation for ODBaseLinkSpec class.

        Owned by:       Craig Carper

        Copyright:      © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

        Change History (most recent first):

                 <7>     6/27/96        DJ              23569: Enable compile on AIX platform
                 <6>      8/3/95        RR              #1257260: Collapse B classes. Remove
                                                                        somInit methods. Don't call IsInitialized
                                                                        or SubclassResponsibility
                 <5>     5/26/95        VL              1251403: Multithreading naming support.
                 <4>     3/10/95        CC              1225926: Added FromThisDraft().
                 <3>    12/20/94        VL              1195012: Make Storage calls be
                                                                        marshallable.
                 <2>    12/15/94        CC              1205709 LinkSpcB.cpp - Added implementation
                                                                        of AcquirePart(), GetPartData(),
                                                                        ReadLinkSpec(), WriteLinkSpec() methods.
                 <1>     6/24/94        CC              first checked in
        To Do:
*/

#define VARIABLE_MACROS

#define ODBaseLinkSpec_Class_Source
#include <LinkSpcB.xih>

#ifndef SOM_ODStorageUnit_xh
#include "StorageU.xh"
#endif

#ifndef _BARRAY_
#include <BArray.h>
#endif

#ifndef _STORUTIL_
#include <StorUtil.h>
#endif

#ifndef _PLATFORM_OS2_
#pragma segment ODBaseLinkSpec
#endif

//==============================================================================
// ODBaseLinkSpec
//==============================================================================

//------------------------------------------------------------------------------
// ODBaseLinkSpec: somInit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSpecsomInit(ODBaseLinkSpec *somSelf)
{
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
    ODBaseLinkSpecData *somThis = ODBaseLinkSpecGetData(somSelf);
#endif
    ODBaseLinkSpecMethodDebug("ODBaseLinkSpec","somInit");

    parent_somInit(somSelf);
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        _fPart = kODNULL;
        _fPartData = CreateEmptyByteArrayStruct(0);
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSpec: somUninit
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSpecsomUninit(ODBaseLinkSpec *somSelf)
{
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
    ODBaseLinkSpecData *somThis = ODBaseLinkSpecGetData(somSelf);
#endif
    ODBaseLinkSpecMethodDebug("ODBaseLinkSpec","somUninit");

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
    DisposeByteArrayStruct(_fPartData);
#endif

    parent_somUninit(somSelf);
}

//------------------------------------------------------------------------------
// ODBaseLinkSpec: InitBaseLinkSpec
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSpecInitBaseLinkSpec(ODBaseLinkSpec *somSelf, Environment *ev,
                ODPart* part,
                ODByteArray* partData)
{
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
    ODBaseLinkSpecData *somThis = ODBaseLinkSpecGetData(somSelf);
#endif
    ODBaseLinkSpecMethodDebug("ODBaseLinkSpec","InitBaseLinkSpec");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        if ( partData )
        {
                DisposeByteArrayStruct(_fPartData);
                _fPartData = CopyByteArrayStruct(partData);
        }

        _fPart = part;
#endif


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSpec::GetPart
//------------------------------------------------------------------------------

SOM_Scope ODPart*  SOMLINK ODBaseLinkSpecGetPart(ODBaseLinkSpec *somSelf, Environment *ev)
{
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
    ODBaseLinkSpecData *somThis = ODBaseLinkSpecGetData(somSelf);
#endif
    ODBaseLinkSpecMethodDebug("ODBaseLinkSpec","GetPart");

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        return _fPart;
#endif
#ifdef _PLATFORM_WIN32_
        return kODNULL;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSpec::GetPartData
//------------------------------------------------------------------------------

SOM_Scope ODByteArray  SOMLINK ODBaseLinkSpecGetPartData(ODBaseLinkSpec *somSelf, Environment *ev)
{
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
    ODBaseLinkSpecData *somThis = ODBaseLinkSpecGetData(somSelf);
#endif
    ODBaseLinkSpecMethodDebug("ODBaseLinkSpec","GetPartData");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return CreateEmptyByteArrayStruct(0);
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        return CopyByteArrayStruct(&_fPartData);
#endif
#ifdef _PLATFORM_WIN32_
        return CreateEmptyByteArrayStruct(0);
#endif


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return CreateEmptyByteArrayStruct(0);
#endif


}

//------------------------------------------------------------------------------
// ODBaseLinkSpec: WriteLinkSpec
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSpecWriteLinkSpec(ODBaseLinkSpec *somSelf, Environment *ev,
                ODStorageUnit* su)
{
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
    ODBaseLinkSpecData *somThis = ODBaseLinkSpecGetData(somSelf);
#endif
    ODBaseLinkSpecMethodDebug("ODBaseLinkSpec","WriteLinkSpec");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

        // Assumes su has been focused to a property and link spec value

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        StorageUnitSetValue(su, ev, sizeof(ODPart*), (ODValue) &_fPart);
        StorageUnitSetValue(su, ev, sizeof(ODULong), (ODValue) &_fPartData._length);
        StorageUnitSetValue(su, ev, _fPartData._length, (ODValue) _fPartData._buffer);
#endif // _PLATFORM_MACINTOSH_ or OS2 or AIX


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSpec: ReadLinkSpec
//------------------------------------------------------------------------------

SOM_Scope void  SOMLINK ODBaseLinkSpecReadLinkSpec(ODBaseLinkSpec *somSelf, Environment *ev,
                ODStorageUnit* su)
{
#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
    ODBaseLinkSpecData *somThis = ODBaseLinkSpecGetData(somSelf);
#endif
    ODBaseLinkSpecMethodDebug("ODBaseLinkSpec","ReadLinkSpec");
#ifdef _PLATFORM_MACINTOSH_
        SOM_CATCH return;
#endif

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_TRY
#endif

#if defined(_PLATFORM_MACINTOSH_) || defined(_PLATFORM_OS2_) || defined(_PLATFORM_AIX_)
        ODPart*                 part;
        ODByteArray             partData;
        ODULong                 dataSize;
        ODULong                 valueSize;

        // Assumes su has been focused to a property and link spec value,
        // and the current offset is where the base link spec was written.
        valueSize = StorageUnitGetValue(su, ev, sizeof(ODPart*), (ODValue) &part);
        if ( valueSize != sizeof(ODPart*) )
                THROW(kODErrCorruptLinkSpecValue);

        valueSize = StorageUnitGetValue(su, ev, sizeof(ODULong), (ODValue) &dataSize);
        if ( valueSize != sizeof(ODULong) )
                THROW(kODErrCorruptLinkSpecValue);

        partData = CreateEmptyByteArrayStruct(dataSize);
        if ( dataSize > 0 )
        {
                valueSize = StorageUnitGetValue(su, ev, dataSize, (ODValue) partData._buffer);
                if ( valueSize != dataSize )
                {
                        DisposeByteArrayStruct(partData);
                        THROW(kODErrCorruptLinkSpecValue);
                }
        }
        partData._length = dataSize;

        _fPart = part;
        DisposeByteArrayStruct(_fPartData);
        _fPartData = partData;
#endif // _PLATFORM_MACINTOSH_ or OS2 or AIX


#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_) || defined(_PLATFORM_AIX_)
        SOM_CATCH_ALL

        SOM_ENDTRY
          return;
#endif
}

//------------------------------------------------------------------------------
// ODBaseLinkSpec::FromThisDraft
//------------------------------------------------------------------------------

SOM_Scope ODBoolean  SOMLINK ODBaseLinkSpecFromThisDraft(ODBaseLinkSpec *somSelf, Environment *ev)
{
    // ODBaseLinkSpecData *somThis = ODBaseLinkSpecGetData(somSelf);
    ODBaseLinkSpecMethodDebug("ODBaseLinkSpec","FromThisDraft");

        return kODTrue;
}
