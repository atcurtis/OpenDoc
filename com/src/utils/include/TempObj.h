//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   BaseTempObj
//		BaseTempRef
//		TempAEDesc
//		TempODByteArray
//		TempODByteArrayStruct
//		TempODHandle
//		TempODHandleLock
//		TempODIText
//		TempODPasteAsResult
//		TempODPtr
//		TempODString
//		TempPlatformFile
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
// @(#) 1.8 com/src/utils/include/TempObj.h, odutils, od96os2, odos29646d 8/19/96 11:57:59 [ 11/15/96 15:29:03 ]
/*
	File:		TempObj.h

	Contains:	Template utilities for exception-safe temporary object references

	Owned by:	Jens Alfke

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	Theory Of Operation:
	
		*** See the Tech Note "Temporary References/Objects" for full documentation.
		
	
		TempObj<T>		T :ODObject				A pointer to a temporary object
		TempRef<T>		T :ODRefCntObject		A pointer to a temporary reference
		
		These are simple template classes that act as a transparent wrapper around
		an OpenDoc object pointer. The temp object can be used wherever a pointer
		to the object would be used. When the temp object goes out of scope, the
		object it wraps will be deleted (in the case of TempObj) or released (in
		the case of TempRef.) This includes the case where an exception is thrown:
		the wrapper is exception-safe.
		
		Example:
		
			RgnHandle r;
			{
				TempODShape s = frame->GetUsedShape(ev,kODNULL);
				r = s->CopyQDRegion(ev);
			}
		
		In this example, s is wrapped around the actual ODShape returned by
		the GetUsedShape method. It is used just as an ODShape* in the next
		line. In the normal course of events, the shape will be released after
		GetQDRegion returns, since s goes out of scope. If GetUsedShape runs
		out of memory and throws an exception, the shape will also be safely
		released.
		
		There is a certain amount of overhead associated with the TempRef
		object. However, the only safe alternative to using it would be to
		wrap an exception handler around the call to GetUsedShape and release
		the shape in the CATCH block (and then reraise the exception) as well
		as after the ENDTRY. This ends up using significantly more code and
		execution time than the TempRef object -- exception handlers are
		quite expensive.

	In Progress:
		
*/

#ifndef _TEMPOBJ_
#define _TEMPOBJ_

#ifndef _ODEXCEPT_
#include <ODExcept.h>
#endif

#ifndef SOM_ODRefCntObject_xh
#include "Part.xh"
#endif

#ifndef SOM_ODWindow_xh
#include "Window.xh"
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif


#ifdef _PLATFORM_MACINTOSH_
class TempODPtr :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class TempODPtr
#else
class TempODPtr : private Destructo
#endif
#endif
{
	public:
	_DLLIMPORTEXPORT_ TempODPtr( );
	_DLLIMPORTEXPORT_ TempODPtr( void *block );
	_DLLIMPORTEXPORT_ ~TempODPtr( );
	operator void* ( )				{return fBlock;}
	void* operator= ( void *b )		{return (fBlock = b);}
	
	protected:
	void* fBlock;

	private: // disallow these:
	TempODPtr(const TempODPtr& );
	void operator=(const TempODPtr& );
};

#ifdef _PLATFORM_MACINTOSH_
class BaseTempObj :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class BaseTempObj
#else
class BaseTempObj : private Destructo
#endif
#endif
{
	public:
	_DLLIMPORTEXPORT_ virtual ~BaseTempObj();
	
	protected:
	ODObject *fObj;
	BaseTempObj( );

	private: // disallow these:
	BaseTempObj(const BaseTempObj& );
	void operator=(const BaseTempObj& );
	// Bitwise assigning one destructo to another smashes
	// a list link with potentially unpleasant effects.
};

#ifdef _PLATFORM_MACINTOSH_
class BaseTempRef :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class BaseTempRef
#else
class BaseTempRef : private Destructo
#endif
#endif
{
	public:
	_DLLIMPORTEXPORT_ void Release();
	_DLLIMPORTEXPORT_ virtual ~BaseTempRef();
	
	protected:
	ODRefCntObject *fObj;
	BaseTempRef( );

	private: // disallow these:
	BaseTempRef(const BaseTempRef& );
	void operator=(const BaseTempRef& );
	// Bitwise assigning one destructo to another smashes
	// a list link with potentially unpleasant effects.
};


#ifdef _USE_TEMPLATES_


	//===========================================================================
	//	TempObj <T>
	//===========================================================================
	
	
	template<class T> class TempObj :public BaseTempObj
	{
		public:
		TempObj( T* );
		T* operator-> ()		{return (T*)fObj;}
		operator T* ()			{return (T*)fObj;}
	
		T* operator= (T* t)		{fObj=t; return t;}
		
		T* DontDelete()			{T* temp=(T*)fObj; fObj=kODNULL; return temp;}
	};
	
	// Implementation of the TempObj constructor:
	template<class T> TempObj<T>::TempObj( T *obj )
	{
		fObj = obj;
	}
	
	
	//===========================================================================
	//	TempRef <T>
	//
	//	Supports a few extra goodies:
	//		Release()		Releases the object and sets the pointer to NULL.
	//		DontRelease()	Sets the pointer to NULL (so the destructor will not
	//							release the object) but returns the old pointer.
	//
	//===========================================================================
	
	
	template<class T> class TempRef :public BaseTempRef
	{
		public:
		TempRef( T* );
		T* operator-> ()		{return (T*)fObj;}
		operator T* ()			{return (T*)fObj;}
		T* operator=( T *t )	{fObj=t; return t;}
		
		T* DontRelease()		{T* temp=(T*)fObj; fObj=kODNULL; return temp;}
	};
	
	
	// Implementation of the TempRef constructor:
	template<class T> TempRef<T>::TempRef( T *obj )
	{
		fObj = obj;
	}


#endif /*_USE_TEMPLATES_*/	


//===========================================================================
//	Instantiations of TempObj and TempRef. Add your own if necessary.
//===========================================================================

class ODFocusSetIterator;
class ODSession;
class ODTypeList;
class ODStorageUnitView;

class ODExtension;
class ODSettingExtension;

class ODFrame;
class ODPart;
class ODShape;
class ODTransform;
class ODStorageUnit;

class ODDraft;
class ODDocument;
class ODContainer;
class ODLink;
class ODLinkSource;
class ODMenuBar;

#ifdef _USE_TEMPLATES_

	typedef TempObj<ODObject>	            TempODObject;
	typedef TempObj<ODSession>		        TempODSession;
	typedef TempObj<ODTypeList>				TempODTypeList;
	typedef TempObj<ODStorageUnitView>		TempODStorageUnitView;
	
	//typedef TempRef<ODExtension>			TempODExtension;
	//typedef TempRef<ODSettingExtension>		TempODSettingExtension;

	typedef TempRef<ODRefCntObject>			TempODRefCntObject;
	typedef TempRef<ODWindow>				TempODWindow;
	typedef TempRef<ODFrame>				TempODFrame;
	typedef TempRef<ODPart>					TempODPart;
	typedef TempRef<ODShape>				TempODShape;
	typedef TempRef<ODTransform>			TempODTransform;
	typedef TempRef<ODStorageUnit>			TempODStorageUnit;

	typedef TempRef<ODDraft>			    TempODDraft;
	typedef TempRef<ODDocument>			    TempODDocument;
	typedef TempRef<ODContainer>			TempODContainer;
	typedef TempRef<ODLink>			        TempODLink;
	typedef TempRef<ODLinkSource>			TempODLinkSource;
	typedef TempRef<ODMenuBar>			    TempODMenuBar;

#else /* not _USE_TEMPLATES_ */

	#ifdef __MWERKS__
		// Make sure 'pragma once' mode is off so the .th files can be included
		// more than once!
		#pragma push
		#pragma once off
	#endif

	/* If not using templates, we rely on the preprocessor. A "template" for an
		instantiation of TempObj is in TempObj.th, and for TempRef in TempRef.th.
		Include one of these headers per instantiation. */

	#define _T_		ODObject
	#define _C_		TempODObject
	#include "TempObj.th"
	#define _T_		ODSession
	#define _C_		TempODSession
	#include "TempObj.th"
	#define _T_		ODTypeList
	#define _C_		TempODTypeList
	#include "TempObj.th"
	#define _T_		ODStorageUnitView
	#define _C_		TempODStorageUnitView
	#include "TempObj.th"
	
	#define _T_		ODRefCntObject
	#define _C_		TempODRefCntObject
	#include "TempRef.th"
	#define _T_		ODWindow
	#define _C_		TempODWindow
	#include "TempRef.th"
	#define _T_		ODFrame
	#define _C_		TempODFrame
	#include "TempRef.th"
	#define _T_		ODPart
	#define _C_		TempODPart
	#include "TempRef.th"
	#define _T_		ODShape
	#define _C_		TempODShape
	#include "TempRef.th"
	#define _T_		ODTransform
	#define _C_		TempODTransform
	#include "TempRef.th"
	#define _T_		ODStorageUnit
	#define _C_		TempODStorageUnit
	#include "TempRef.th"

	#define _T_		ODDraft
	#define _C_		TempODDraft
	#include "TempRef.th"
	#define _T_		ODDocument
	#define _C_		TempODDocument
	#include "TempRef.th"
	#define _T_		ODContainer
	#define _C_		TempODContainer
	#include "TempRef.th"
	#define _T_		ODLink
	#define _C_		TempODLink
	#include "TempRef.th"
	#define _T_		ODLinkSource
	#define _C_		TempODLinkSource
	#include "TempRef.th"
	#define _T_		ODMenuBar
	#define _C_		TempODMenuBar
	#include "TempRef.th"

/*
	#define _T_		ODExtension
	#define _C_		TempODExtension
	#include "TempRef.th"
	#define _T_		ODSettingsExtension
	#define _C_		TempODSettingsExtension
	#include "TempRef.th"
*/
	#ifdef __MWERKS__
		#pragma pop
	#endif

#endif /*_USE_TEMPLATES_*/


//===========================================================================
//	Temp strings.
//===========================================================================

#ifdef _PLATFORM_MACINTOSH_
class TempODString :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class TempODString
#else
class TempODString : private Destructo
#endif
#endif
{
	protected:
	char* fStr; 

	public:
	_DLLIMPORTEXPORT_ ~TempODString();
	_DLLIMPORTEXPORT_ TempODString( char* );
	operator char* ()         { return fStr; }

	char* operator= (char* s) { fStr = s; return s; }
	
	char* DontDelete()        { char* s = fStr; fStr = kODNULL; return s;}

	private: // disallow these:
	TempODString( );
	TempODString(const TempODString& );
	void operator=(const TempODString& );
};

// typedef string  ODISOStr;
	// typedef ODISOStr ODContainerType;
	// typedef ODISOStr  ODPropertyName;
	// typedef ODISOStr  ODStorageUnitName;
	// typedef	ODISOStr	ODDraftName;

	// typedef ODISOStr  ODType;
		// typedef ODType    ODValueType;
		// typedef ODType   ODObjectType;
		// typedef ODType   ODFocusType;

// typedef char* ODEditor;


typedef TempODString TempODISOStr;
typedef TempODString TempODType;
typedef TempODString TempODValueType;
typedef TempODString TempODObjectType;
typedef TempODString TempODFocusType;
typedef TempODString TempODContainerType;
typedef TempODString TempODPropertyName;
typedef TempODString TempODStorageUnitName;
typedef TempODString TempODDraftName;
typedef TempODString TempStringPtr;

typedef TempODString TempODEditor;

#ifdef __PRIVATE__
//===========================================================================
//	Temp handles.
//===========================================================================

#ifdef _PLATFORM_MACINTOSH_
class TempODHandle :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class TempODHandle
#else
class TempODHandle : private Destructo
#endif
#endif
{
	protected:
	ODHandle fHandle; 

	public:
	~TempODHandle();
	TempODHandle( ODHandle );
	operator ODHandle ()           { return fHandle; }

	ODHandle operator= (ODHandle h)
	{ fHandle = h; return h; }
	
	ODHandle DontDelete()
	{ ODHandle h = fHandle; fHandle = kODNULL; return h;}

	private: // disallow these:
	TempODHandle( );
	TempODHandle(const TempODHandle& );
	void operator=(const TempODHandle& );
};

#ifdef _PLATFORM_MACINTOSH_
class TempODHandleLock :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class TempODHandleLock
#else
class TempODHandleLock : private Destructo
#endif
#endif
{
	protected:
	ODHandle fHandle; 

	public:
	~TempODHandleLock();            // unlocks the handle
	TempODHandleLock( ODHandle );   // locks the handle
	operator ODHandle ()           { return fHandle; }

	ODHandle operator= (ODHandle h)
	{ fHandle = h; return h; }

	private: // disallow these:
	TempODHandleLock( );
	TempODHandleLock(const TempODHandleLock& );
	void operator=(const TempODHandleLock& );
};

//===========================================================================
//	Temp platform files.
//===========================================================================

class PlatformFile;

#ifdef _PLATFORM_MACINTOSH_
class TempPlatformFile :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class TempPlatformFile
#else
class TempPlatformFile : private Destructo
#endif
#endif
{
	protected:
	PlatformFile* fFile; 

	public:
	~TempPlatformFile();
	TempPlatformFile( PlatformFile* );
	PlatformFile* operator-> ()         { return fFile; }
	operator PlatformFile* ()           { return fFile; }

	PlatformFile* operator= (PlatformFile* f)
	{ fFile = f; return f; }
	
	PlatformFile* DontDelete()
	{ PlatformFile* f = fFile; fFile = kODNULL; return f;}

	private: // disallow these:
	TempPlatformFile( );
	TempPlatformFile(const TempPlatformFile& );
	void operator=(const TempPlatformFile& );
};
#endif // __PRIVATE__

//===========================================================================
//	Temp ODPasteAsResult.
//===========================================================================

// struct ODPasteAsResult; // from ODTypesM.xh

#ifdef _PLATFORM_MACINTOSH_
class TempODPasteAsResult :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class TempODPasteAsResult
#else
class TempODPasteAsResult : private Destructo
#endif
#endif
{
	protected:
	ODPasteAsResult* fResult; 

	public:
	_DLLIMPORTEXPORT_ ~TempODPasteAsResult();
	_DLLIMPORTEXPORT_ TempODPasteAsResult( ODPasteAsResult* );
	ODPasteAsResult* operator-> ()         { return fResult; }
	operator ODPasteAsResult* ()           { return fResult; }

	ODPasteAsResult* operator= (ODPasteAsResult* r)
	{ fResult = r; return r; }
	
	ODPasteAsResult* DontDelete()
	{ ODPasteAsResult* r = fResult; fResult = kODNULL; return r;}

	private: // disallow these:
	TempODPasteAsResult( );
	TempODPasteAsResult(const TempODPasteAsResult& );
	void operator=(const TempODPasteAsResult& );
};


#ifdef _PLATFORM_MACINTOSH_
//===========================================================================
//	Temp AEDescs.
//===========================================================================

struct AEDesc;

class TempAEDesc :Destructo
{
	protected:
	AEDesc* fDesc; 

	public:
	~TempAEDesc();
	TempAEDesc( AEDesc* );
	AEDesc* operator-> ()         { return fDesc; }
	operator AEDesc* ()           { return fDesc; }

	AEDesc* operator= (AEDesc* d)
	{ fDesc = d; return d; }
	
	AEDesc* DontDelete()
	{ AEDesc* d = fDesc; fDesc = kODNULL; return d;}

	private: // disallow these:
	TempAEDesc( );
	TempAEDesc(const TempAEDesc& );
	void operator=(const TempAEDesc& );
};
#endif


//===========================================================================
//	Temp ODIText.
//===========================================================================

class ODIText;

#ifdef _PLATFORM_MACINTOSH_
class TempODIText :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class TempODIText
#else
class TempODIText : private Destructo
#endif
#endif
{
	protected:
	ODIText* fText; 

	public:
	_DLLIMPORTEXPORT_ ~TempODIText();
	_DLLIMPORTEXPORT_ TempODIText( ODIText* );
	ODIText* operator-> ()          { return fText; }
	operator ODIText* ()            { return fText; }

	ODIText* operator= (ODIText* t)
	{ fText = t; return t; }
	
	ODIText* DontDelete()
	{ ODIText* t = fText; fText = kODNULL; return t;}

	private: // disallow these:
	TempODIText( );
	TempODIText(const TempODIText& );
	void operator=(const TempODIText& );
};

// typedef ODIText ODName; 
// typedef ODName  ODDocumentName;

typedef TempODIText TempODName;
typedef TempODIText TempODDocumentName;

//===========================================================================
//	Temp byte arrays.
//===========================================================================

#ifdef _PLATFORM_MACINTOSH_
class TempODByteArray :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class TempODByteArray
#else
class TempODByteArray : private Destructo
#endif
#endif
{
	protected:
	ODByteArray* fBA; 

	public:
	_DLLIMPORTEXPORT_ ~TempODByteArray();
	_DLLIMPORTEXPORT_ TempODByteArray( ODByteArray* );
	ODByteArray* operator-> ()          { return fBA; }
	operator ODByteArray* ()            { return fBA; }

	ODByteArray* operator= (ODByteArray* ba) 
	{ fBA = ba; return ba; }
	
	ODByteArray* DontDelete()           
	{ ODByteArray* ba = fBA; fBA = kODNULL; return ba;}

	private: // disallow these:
	TempODByteArray( );
	TempODByteArray(const TempODByteArray& );
	void operator=(const TempODByteArray& );
};

#ifdef _PLATFORM_MACINTOSH_
class TempODByteArrayStruct :Destructo
#else
#ifdef _NATIVE_EXCEPTIONS_  // OD_BUG?
class TempODByteArrayStruct
#else
class TempODByteArrayStruct : private Destructo
#endif
#endif
{
	protected:
	ODByteArray fBA; 

	public:
	_DLLIMPORTEXPORT_ ~TempODByteArrayStruct();
	_DLLIMPORTEXPORT_ TempODByteArrayStruct( );
	_DLLIMPORTEXPORT_ TempODByteArrayStruct( ODByteArray ba );
	ODByteArray* operator-> ()          { return &fBA; }
	operator ODByteArray* ()            { return &fBA; }

	ODByteArray operator= ( ODByteArray ba ) 
	{ fBA = ba; return ba; }
	
	void DontDelete()           
	{ fBA._buffer = kODNULL;}

	private: // disallow these:
	TempODByteArrayStruct(const TempODByteArrayStruct& );
	void operator=(const TempODByteArrayStruct& );
};

// typedef ODByteArray ODContainerID;
// typedef ODByteArray ODActionData;
// typedef ODByteArray ODContour;
// typedef ODByteArray ODPolygon;

typedef TempODByteArray TempODContainerID;
typedef TempODByteArray TempODActionData;
typedef TempODByteArray TempODContour;
// typedef TempODByteArray TempODPolygon; // use ODTempPolygon instead

typedef TempODByteArrayStruct TempODContainerIDStruct;
typedef TempODByteArrayStruct TempODActionDataStruct;
typedef TempODByteArrayStruct TempODContourStruct;
// typedef TempODByteArrayStruct TempODPolygonStruct;


#endif /*_TEMPOBJ_*/
