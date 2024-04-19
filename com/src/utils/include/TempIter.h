//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   TempODDragItemIterator
//		TempODEmbeddedFramesIterator
//		TempODFacetIterator
//		TempODFocusSetIterator
//		TempODFrameFacetIterator
//		TempODTypeListIterator
//		TempODWindowIterator
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
// @(#) 1.4 com/src/utils/include/TempIter.h, odutils, od96os2, odos29646d 8/19/96 11:57:04 [ 11/15/96 15:29:02 ]
/*
	File:		TempIter.h

	Contains:	Template utilities for OpenDoc iterators

	Owned by:	Jens Alfke

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.
	
	Theory of Operation:
	
		These are specific TempObj utilities (see the "Temporary Objects"
		tech note) for OpenDoc iterators. They are intended to be used in
		this fashion:
		
			extern void DoSomethingWith( ODFoo* );
			extern void OrSomethingWith( ODFoo* );
			...
			ODBazz *bazz;
			...
			for( TempODFooIterator iter(ev,bazz); iter.Current(); iter.Next() ) {
				DoSomethingWith(iter);
				OrSomethingWith(iter.Current());
				// within the loop you can use iter.Current() or just iter
				// to refer to the current object it's pointing to.
			}
		
		True C++meisters can even do the for loop like this:
		
			for( TempODFooIterator iter(ev,bazz); iter; iter++ )

		If you wish you can also call First() to reset the iterator to
		the beginning and return the first element.
			
*/


#ifndef _TEMPITER_
#define _TEMPITER_


#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifndef SOM_ODWindow_xh
#include <Window.xh>
#endif

#ifndef SOM_ODDragAndDrop_xh
#include <DragDrp.xh>
#endif

#ifndef _ODMVCLNK_
#include <ODMvcLnk.h>
#endif


struct Environment;
class ODFrame;
class ODFacet;
class ODFocusSet;
class ODTypeList;


class TempODFacetIterator :public BaseTempObj
{
	public:
	_DLLIMPORTEXPORT_ TempODFacetIterator( Environment*, ODFacet*, ODTraversalType, ODSiblingOrder );
	_DLLIMPORTEXPORT_ ODFacet*	First( );
	ODFacet*	Current( )		{return fCurrent;}
	_DLLIMPORTEXPORT_ ODFacet*	Next( );
	_DLLIMPORTEXPORT_ ODBoolean	IsNotComplete( );
			operator ODFacet*()	{return fCurrent;}
	ODFacet*	operator ->()		{return fCurrent;}
	ODFacet*	operator ++()		{return this->Next();}
	
	private:
	Environment* fEv;
	ODFacet*	 fCurrent;
};


class TempODFrameFacetIterator :public BaseTempObj
{
	public:
	_DLLIMPORTEXPORT_ TempODFrameFacetIterator( Environment*, ODFrame* );
	_DLLIMPORTEXPORT_ ODFacet*	First( );
	ODFacet*	Current( )				{return fCurrent;}
	_DLLIMPORTEXPORT_ ODFacet*	Next( );
	_DLLIMPORTEXPORT_ ODBoolean	IsNotComplete( );
				operator ODFacet*()		{return fCurrent;}
	ODFacet*	operator ->()			{return fCurrent;}
	ODFacet*	operator ++()			{return this->Next();}
	
	private:
	Environment* fEv;
	ODFacet*	 fCurrent;
};


class TempODEmbeddedFramesIterator :public BaseTempObj
{
	public:
	_DLLIMPORTEXPORT_ TempODEmbeddedFramesIterator( Environment*, ODPart*, ODFrame* );
	_DLLIMPORTEXPORT_ ODFrame*	First( );
	ODFrame*	Current( )			{return fCurrent;}
	_DLLIMPORTEXPORT_ ODFrame*	Next( );
	_DLLIMPORTEXPORT_ ODBoolean	IsNotComplete( );
			operator ODFrame*()		{return fCurrent;}
	ODFrame*	operator ->()			{return fCurrent;}
	ODFrame*	operator ++()			{return this->Next();}
	
	private:
	Environment* fEv;
	ODFrame*	 fCurrent;
};


class TempODWindowIterator :public BaseTempObj
{
	public:
	_DLLIMPORTEXPORT_ TempODWindowIterator( Environment*, ODWindowState* );
	_DLLIMPORTEXPORT_ ODWindow*	First( );
	ODWindow*	Current( )			{return fCurrent;}
	_DLLIMPORTEXPORT_ ODWindow*	Next( );
	_DLLIMPORTEXPORT_ ODBoolean	IsNotComplete( );
				operator ODWindow*()	{return fCurrent;}
	ODWindow*	operator ->()			{return fCurrent;}
	ODWindow*	operator ++()			{return this->Next();}
	
	private:
	Environment* fEv;
	ODWindow*	 fCurrent;
};


class TempODFocusSetIterator :public BaseTempObj
{
	public:
	_DLLIMPORTEXPORT_ TempODFocusSetIterator( Environment*, ODFocusSet* );
	_DLLIMPORTEXPORT_ ODTypeToken	First( );
	ODTypeToken	Current( )			{return fCurrent;}
	_DLLIMPORTEXPORT_ ODTypeToken	Next( );
	_DLLIMPORTEXPORT_ ODBoolean	IsNotComplete( );

			operator ODTypeToken()	{return fCurrent;}
	ODTypeToken	operator ++()			{return this->Next();}
	
	private:
	Environment* fEv;
	ODTypeToken	 fCurrent;
};


class TempODTypeListIterator :public BaseTempObj
{
	public:
	_DLLIMPORTEXPORT_ TempODTypeListIterator( Environment*, ODTypeList* );
	_DLLIMPORTEXPORT_ ~TempODTypeListIterator( );
	_DLLIMPORTEXPORT_ ODType		First( );
	ODType		Current( )			{return fCurrent;}
	_DLLIMPORTEXPORT_ ODType		Next( );
	_DLLIMPORTEXPORT_ ODBoolean	IsNotComplete( );
			operator ODType()		{return fCurrent;}
	ODType		operator++()			{return this->Next();}
	
	private:
	Environment* fEv;
	ODType		 fCurrent;
};


#ifdef _OD_IMPL_

class TempODDragItemIterator :public BaseTempObj
{
	public:
	TempODDragItemIterator( Environment*, ODDragAndDrop* );
	ODStorageUnit* First( );
	ODStorageUnit* Current( )				{return fCurrent;}
	ODStorageUnit* Next( );
	ODBoolean	IsNotComplete( );
				operator ODStorageUnit*()	{return fCurrent;}
	ODStorageUnit* operator ->()			{return fCurrent;}
	ODStorageUnit* operator ++()			{return this->Next();}
	
	private:
	Environment* fEv;
	ODStorageUnit* fCurrent;
};

#endif /*_OD_IMPL_*/


#endif /*_TEMPITER_*/
