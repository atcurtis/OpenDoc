//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
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
/* @(#) 1.4 com/src/utils/SEUtils.cpp, odutils, od96os2, odos29646d 10/30/96 20:23:18 [11/15/96 15:28:52] */
/*
	File:		SEUtils.cpp

	Contains:	Semantic Event Utility Functions implementation.

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
		
*/

#if !defined(_PLATFORM_WIN32_)
#ifndef __ERRORS__
#include <Errors.h>
#endif

#ifndef _ODDESUTL_
#include <ODDesUtl.h>
#endif

#ifndef SOM_ODAppleEvent_xh
#include "ODAplEvt.xh"
#endif

#ifndef SOM_ODOSLToken_xh
#include "ODOSLTkn.xh"
#endif

#ifndef SOM_ODNameResolver_xh
#include "NamRslvr.xh"
#endif

#ifndef __FILES__
#include <Files.h>
#endif
#endif // !defined(_PLATFORM_WIN32_)

#ifndef _EXCEPT_
#include "Except.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef __STRING__
#include <string.h>
#endif

#ifndef __STDLIB__
#include <stdlib.h>
#endif

#if !defined(_PLATFORM_WIN32_)
#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __AEOBJECTS__
#include <AEObjects.h>
#endif
#endif // !defined(_PLATFORM_WIN32_)

#pragma segment ODSEUtils

#ifndef _SEUTILS_
#include "SEUtils.h"
#endif

#if !defined(_PLATFORM_WIN32_)
#ifndef _SIHLPABS_
#include "SIHlpAbs.h"
#endif

#ifndef _TEMPITER_
#include "TempIter.h"
#endif
#endif // !defined(_PLATFORM_WIN32_)

#ifndef SOM_ODEmbeddedFramesIterator_xh
#include "EmbFrItr.xh"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef SOM_Module_OpenDoc_StdDefs_defined
#include <StdDefs.xh>
#endif


#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
OSErr MyAECoerceDescPtr(AEDesc theAEDesc, DescType toType, Ptr dataPtr,
    					Size maximumSize, Size* actualSize);
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
long MyRandom(long count);

ODBoolean MissingParams(const AppleEvent* message)
{
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	AEKeyword 	missedKeyword;
	DescType	ignoreType;
	Size		ignoreSize;
	OSErr 		myErr;
	
	myErr = AEGetAttributePtr(message, keyMissedKeywordAttr, typeWildCard,
								&ignoreType, (Ptr)&missedKeyword,
								sizeof(missedKeyword), &ignoreSize);
	return myErr == noErr;
#else // defined(_PLATFORM_WIN32_) && defined(PORTING_HACK)
	return kODFalse;
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
}

/* -- I've made this a memeber function of ODApplicatiin (in Shell.cp)
// HEY! This function assumes that typeCode is typeChar and toType is typeFSS.
//pascal OSErr Pathname2FSSpec(DescType		typeCode,
//								Ptr			dataPtr,
//								Size		dataSize,
//								DescType	toType,
//								long		handlerRefCon,
//								AEDesc*		result)


// ODDescCoercionHandler 
void Pathname2FSSpec(	ODPart* thePart, DescType typeCode, Ptr dataPtr,
						Size dataSize, DescType	toType, long handlerRefCon,
						AEDesc*	 result)
{
	Str255		fileName;
	FSSpec		fileSpec;
	const short	kNoVRefNum = 0;
	const long	kNoDirID = 0;
	OSErr		error;
	
	if (dataSize > sizeof(Str255))
		return bdNamErr;

	ODBlockMove(dataPtr, fileName + 1, dataSize);
	fileName[0] = (unsigned char)dataSize;

	error = FSMakeFSSpec(kNoVRefNum, kNoDirID, fileName, &fileSpec);
	THROW_IF_ERROR( error );
	
	error = AECreateDesc(typeFSS, (Ptr) &fileSpec, sizeof(fileSpec), result);
	THROW_IF_ERROR( error );
}

*/

/*
The following functions were lifted from Quill.p and translated
*/

#define genericErr (-1799)
#define errAEBadData (-15310)
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
Size gActSize;
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)


OSErr DecodeOrdinal(AEDesc ordData, long count, long* index,
    				Boolean* allFlag, Boolean* zeroFlag)
/* this routine is used whenever an element is specified by an absolute position
  within a sequence of elements - such as "word 3 of window 'johnson'", "any line
  of item 17 of window 'Kelvin'", etc.  The data specifying the position can be
  a positive integer (which just means the actual position of the element in the
  sequence: 1 for the first element, 2 for the second, etc.), a negative integer
  (which indicates position relative to the last element of the sequence: -1 is
  the last element, -2 the next to last, etc.), or a descriptor of typeAbsoluteOrdinal:
  kAEFirst, kAELast, kAEMiddle, kAEAny, or kAEAll.  
  
  DecodeOrdinal takes the data specifying the position, and a count of all the elements
  in the sequence under consideration, and returns (whenever possible) a positive integer
  (in the return VAR index) representing the actual position of the element in the sequence 
  (1 for first, 2 for second, etc.).  It also returns flags indicating (a) whether the 
  ordinal was kAEAll and (b) whether the count was 0; both of these are conditions that 
  many calling routines will have to special-case.  In the case of kAEAll, the return VAR 
  index is set to the count; in the case of count = 0, the return VAR index is set to 0.
  
  There are a few error conditions:  (a) count < 0; (b) bad ordData (not an integer, and
  not one of the five defined absolute ordinal specifiers).  **CHECK - should "integer ordData
  out of range" (for example, an integer > count, or < -count, or = 0) be an error,
  or should we allow that so that people can talk about "the hypothetical element
  beyond the last", or whatever?  For now, let's make that a non-error.
  
  INPUTS:	ordData		a descriptor that specifies an ordinal - either
  						an integer (positive or negative) or something
						of typeAbsoluteOrdinal
			count		the total number of elements in the group involved
						(number of windows, number or chars, or whatever)
			index		return VAR for the actual position being described
			allFlag		return VAR: TRUE if the ordData was kAEAll, FALSE o.w.
			zeroFlag	return VAR: TRUE if the count was zero (for many
						calling routines this is an error condition), FALSE o.w.
  OUTPUTS:	error code (noErr if none)
*/
{
	OSErr myErr;
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	DescType absOrd;
	long intOrd;

  myErr = genericErr;
  *index = count;
  *allFlag = kODFalse;
  
  *zeroFlag = (count == 0);
  if (count < 0)
  {
	  myErr = errAEBadData;
	  goto exit;
	}
  
  myErr = MyAECoerceDescPtr(ordData, typeAbsoluteOrdinal, (Ptr)&absOrd,
  							sizeof(absOrd), &gActSize);
  if (myErr == noErr)
    {
	  /* got an absolute ordinal */
	  /* note that, as we enter here, index == count and myErr == noErr */
	  *allFlag = (absOrd == kAEAll);
	  if (*allFlag)
	  	goto exit;	/* finish up */
		
	  if ((absOrd != kAEFirst) && (absOrd != kAELast)
	  		&& (absOrd != kAEMiddle) && (absOrd != kAEAny))
	    {
		  myErr = errAEBadData;
		  goto exit;
		}
		
	  if (*zeroFlag || (absOrd == kAELast))
	  	goto exit; /* in both cases, index == count (already done) */
		
	  if (absOrd == kAEFirst)
	  	*index = 1;
	  else if (absOrd == kAEMiddle)
	  	*index = (count + 1) / 2;
	  else
	  	*index = MyRandom(count);
	  
	  goto exit;
	}	/* of absolute ordinal */
	
  /* try actual integer */
  myErr = MyAECoerceDescPtr(ordData, typeLongInteger, (Ptr)&intOrd,
  								sizeof(intOrd), &gActSize);
  if (myErr)
	goto exit;
	  
  if (intOrd < 0)
  	*index = count + intOrd + 1;	/* e.g., intOrd == -1 means index == count */
  else
  	*index = intOrd;
  
  /* should we validate index here (wrt count)?  let's skip it for now */
  
exit:	/* finish up */
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
  return myErr;
}

#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
OSErr MyAECoerceDescPtr(AEDesc theAEDesc, DescType toType, Ptr dataPtr,
    					Size maximumSize, Size* actualSize)
/* this routine plugs a hole that's been nagging at me in the AppleEvents
  interface.  It takes a descriptor and coerces it to a desired type; but
  instead of returning a descriptor, it returns data in a buffer specified
  by the caller.
  INPUTS:	theAEDesc		descriptor to be coerced
  			toType			type to coerce it to
			dataPtr			ptr to data buffer
			maximumSize		maximum length in bytes of data to be returned
			actualSize		actual length in bytes of data for the descriptor
  OUTPUTS:	error code (noErr if none)
  ERRORS:
  SIDE EFFECTS:
  NOTES:	12/16/91	BHM		(1) Changed to avoid unecessary duplication when the type
  								doesn't really change (this should also enable it to handle
								typeWildCard better)
								(2) We don't need to dispose of newDesc because it is a direct
								copy (not a duplicate) of either theAEDesc or resultDesc - that
								is, it contains the same handle
			03/12/92	BHM		Added special case to deal with NIL data handles (which should only 
								occur for typeNull, I believe)
*/
{
	short myErr;
	AEDesc newDesc;
    AEDesc resultDesc = NULL_DESCRIPTOR_DEFINITION;
	Size transferSize;

  myErr = errAECoercionFail;
  
  /* to avoid unnecessary duplication, check old type vs. new type */
  if ((theAEDesc.descriptorType == toType) || (toType == typeWildCard))
      newDesc = theAEDesc;
  else
    {
	  /* must coerce to new type */
	  myErr = AECoerceDesc(&theAEDesc, toType, &resultDesc); 
	  if (myErr)
	  	goto exit;
	  newDesc = resultDesc;
	}
  	  
//  WITH newDesc DO
    {
	  /* special-case for NIL handle */	/* should I **CHECK to make sure it's typeNull here? */
	  if (newDesc.dataHandle == kODNULL)
	    {
		  actualSize = 0;
		  myErr = noErr;
		  goto exit;
		}
		
	  /* not NIL handle - get the size */
      *actualSize = ODGetHandleSize((ODHandle)newDesc.dataHandle);
		myErr = MemError();
		if (myErr)
	  		goto exit;
	
	  /* calculate number of bytes to move */
	  transferSize = *actualSize;
	  if (maximumSize < transferSize)
	  	transferSize = maximumSize;
	
	  /* move the data */
//	  HLock(newDesc.dataHandle);
	  ODBlockMove(*(newDesc.dataHandle), dataPtr, transferSize);
//	  HUnlock(newDesc.dataHandle);
	}	/* of WITH newDesc */
	
  /* everything fine */
  myErr = noErr;
	
exit:	/* finish up */
  AEDisposeDesc(&resultDesc);
  return myErr;
}	/* MyAECoerceDescPtr */

long MyRandom(long count)
/* return a random integer between 1 and count, inclusive
  INPUTS:	count		upper bound for random number
  OUTPUTS:	a random integer between 1 and count, inclusive
  NOTES:	**CHECK - this is a quick, dirty, and WRONG version
  			to be used for testing only
*/
{
long longRandom;

  longRandom = 0x10000*Random() + Random();
  return (labs(longRandom) % count) + 1;
}	/* MyRandom */
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)

//------------------------------------------------------------------------------
// GetSLongAttr
//------------------------------------------------------------------------------

ODSLong GetSLongAttr(AppleEvent* ae, AEKeyword keyword)
{
	OSErr		result;
	ODSLong	value;
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	DescType	actualType;
	Size		sizeOfBuffer = sizeof(value);
	Size		actualSize;

	result = AEGetAttributePtr(ae, keyword, typeWildCard, &actualType,
									(Ptr)&value, sizeOfBuffer, &actualSize);
	if (result != noErr)
		THROW((ODError)result);
	//if ((actualType != typeShortInteger) || (actualSize != sizeOfBuffer))
	if (actualSize != sizeOfBuffer)
		THROW(kODErrOutOfMemory);
		
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	return value;
}

//------------------------------------------------------------------------------
// GetSLongAttrOD
//------------------------------------------------------------------------------

ODSLong GetSLongAttrOD(ODAppleEvent* ae, AEKeyword keyword)
{
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	AppleEvent realAE;
	THROW_IF_ERROR( ODDescToAEDesc( ae, &realAE ) ) ;
	ODSLong result = GetSLongAttr( &realAE, keyword );
	(void)AEDisposeDesc( &realAE );
	return result;
#else // defined(_PLATFORM_WIN32_) && defined(PORTING_HACK)
	return 0;
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
}

//------------------------------------------------------------------------------
// ThrowIfCantCoerce
//------------------------------------------------------------------------------

// does an in-place coercion!
void ThrowIfCantCoerce( AEDesc* data, DescType desiredType )
{
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	if ( data->descriptorType == desiredType )
		return ;

	AEDesc newDesc ;
	THROW_IF_ERROR( AECoerceDesc( data, desiredType, &newDesc ) ) ;
	AEDisposeDesc( data ) ;
	*data = newDesc ;
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
}

//------------------------------------------------------------------------------
// ThrowIfNotAbsent
//------------------------------------------------------------------------------

void ThrowIfNotAbsent( OSErr err )
{
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	if ( (err == noErr) || ( err == errAEDescNotFound ) )
		return ;
	THROW( err ) ;
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
}

//------------------------------------------------------------------------------
// UpdateUserToken
//------------------------------------------------------------------------------

void UpdateUserToken(Environment* ev, ODNameResolver* resolver,
						ODOSLToken* odToken, AEDesc* desc)
{
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	ODDesc*	userODToken = resolver->GetUserToken(ev, odToken);
	THROW_IF_ERROR(AEDescToODDesc(desc, userODToken));
	THROW_IF_ERROR(AEDisposeDesc(desc));
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
}

//------------------------------------------------------------------------------
// CountEmbeddedParts
//------------------------------------------------------------------------------

ODSLong CountEmbeddedParts(Environment* ev, ODPart* prt)
{
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	if ( prt == kODAppShell ) return 1;

	ODSLong result = 0;
	for ( TempODEmbeddedFramesIterator iter(ev,prt,kODNULL);
			iter.Current(); iter.Next() )
	{
		++result;
	}
	return result;
#else // defined(_PLATFORM_WIN32_) && defined(PORTING_HACK)
	return 0;
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
}

//------------------------------------------------------------------------------
// ODDisposeAppleEvent
//------------------------------------------------------------------------------

OSErr ODDisposeAppleEvent( AppleEvent* aevt )
{
#if !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
	(void)AEDisposeDesc( aevt );
	return AEDisposeDesc( aevt );
#else // defined(_PLATFORM_WIN32_) && defined(PORTING_HACK)
	return 0;
#endif // !defined(_PLATFORM_WIN32_) || !defined(PORTING_HACK)
}


