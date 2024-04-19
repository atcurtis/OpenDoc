/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odmri
 *
 *   CLASSES: none
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
// @(#) 1.6 com/src/utils/include/StdTypIO.h, odmri, od96os2, odos29646d 8/20/96 07:46:26 [ 11/15/96 15:29:02 ]
/*
	File:		StdTypIO.h

	Contains:	functions for read/writing standard typed values from/to storage units

	Owned by:	Tantek ‚elik

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.
		
*/

#ifndef _STDTYPIO_
#define _STDTYPIO_


#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif

//==============================================================================
// Classes used by this interface
//==============================================================================

#ifdef __cplusplus
	class	ODStorageUnit;
	class	ODTypeList;
#else
	#ifndef SOM_ODStorageUnit_h
	#include <StorageU.h>
	#endif
#endif

//==============================================================================
// Function prototypes
//==============================================================================

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import on
#endif

#ifdef __cplusplus
extern "C" {
#endif

_DLLIMPORTEXPORT_ ODBoolean	ODGetBooleanProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val);
_DLLIMPORTEXPORT_ void		ODSetBooleanProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODBoolean value);

_DLLIMPORTEXPORT_ ODUShort	ODGetUShortProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val);
_DLLIMPORTEXPORT_ void		ODSetUShortProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODUShort value);

_DLLIMPORTEXPORT_ ODSShort	ODGetSShortProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val);
_DLLIMPORTEXPORT_ void		ODSetSShortProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODSShort value);

_DLLIMPORTEXPORT_ ODULong		ODGetULongProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val);
_DLLIMPORTEXPORT_ void		ODSetULongProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODULong value);

_DLLIMPORTEXPORT_ ODSLong		ODGetSLongProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val);
_DLLIMPORTEXPORT_ void		ODSetSLongProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODSLong value);

_DLLIMPORTEXPORT_ ODISOStr	ODGetISOStrProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODISOStr value, ODULong* size);
_DLLIMPORTEXPORT_ void		ODSetISOStrProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODISOStr value);

_DLLIMPORTEXPORT_ void		ODGetTypeListProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODTypeList* typeList);
_DLLIMPORTEXPORT_ void		ODSetTypeListProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODTypeList* typeList);

// ODGetITextProp works as follows: the value returned is that of the
// name param passed in if name is not null.  If name is null, a structure of
// the necessary size is allocated.

_DLLIMPORTEXPORT_ ODIText*	ODGetITextProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODIText* name);
_DLLIMPORTEXPORT_ void		ODSetITextProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODIText* name);

_DLLIMPORTEXPORT_ ODTime		ODGetTime_TProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val);
_DLLIMPORTEXPORT_ void		ODSetTime_TProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODTime value);

_DLLIMPORTEXPORT_ ODPoint*	ODGetPointProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODPoint* value);
_DLLIMPORTEXPORT_ void		ODSetPointProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODPoint* value);

_DLLIMPORTEXPORT_ ODRect*		ODGetRectProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODRect* value);
_DLLIMPORTEXPORT_ void		ODSetRectProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODRect* value);

_DLLIMPORTEXPORT_ ODID		ODGetStrongSURefProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val);
_DLLIMPORTEXPORT_ void		ODSetStrongSURefProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODID id);

_DLLIMPORTEXPORT_ ODID		ODGetWeakSURefProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val);
_DLLIMPORTEXPORT_ void		ODSetWeakSURefProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODID id);

_DLLIMPORTEXPORT_ ODPolygon*	ODGetPolygonProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODPolygon* value);
_DLLIMPORTEXPORT_ void		ODSetPolygonProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				const ODPolygon* value);

_DLLIMPORTEXPORT_ ODMatrix*	ODGetMatrixProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODMatrix* value);
_DLLIMPORTEXPORT_ void		ODSetMatrixProp(Environment* ev,
				ODStorageUnit* su, ODPropertyName prop, ODValueType val,
				ODMatrix* value);

#ifdef _PLATFORM_MACINTOSH_
//--------------------------------------------------------------------
// Icon Family I/O
//--------------------------------------------------------------------

// Note: ODIconFamily is a platform specific type, so these two functions
// will have platform specific implementations. -T‚

enum {							// For use with iconMask parameter:
	kAllIconsMask	 = 0xFFFFFFFF,	// All icons usable on this platform
	kBWIconsMask 	 = 0x0421		// 1 bit deep, 16,32,64 pixels wide
};

/*	Expected values for the ODValueType begin with "OpenDoc:Type:IconFamily:"
	followed by the name of a platform. These are defined in StdTypes.idl as
	kODIconFamilyMac, kODIconFamilyWin, kODIconFamilyOS2, kODIconFamilyAIX, etc.
	If you only specify kODIconFamily, the current platform's type will be used.
*/

ODIconFamily	ODGetIconFamilyProp(Environment* ev,
					ODStorageUnit* su, ODPropertyName prop, 
					ODValueType val, ODULong iconMask);
void			ODSetIconFamilyProp(Environment* ev,
					ODStorageUnit* su, ODPropertyName prop, 
					ODValueType val, ODIconFamily iconFamily,
					ODBoolean deleteOtherPlatformIcons);
#endif  // MACINTOSH

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef _OD_IMPL_SHARE_UTILS_
#pragma import off
#endif

#endif // _STDTYPIO_
