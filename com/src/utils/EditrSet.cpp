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
// @(#) 1.5 com/src/utils/EditrSet.cpp, odutils, od96os2, odos29646d 7/15/96 17:57:52 [ 11/15/96 15:28:43 ]
/*
	File:		EditrSet.cpp

	Contains:	C++ Implementation for EditorSet and EditorSetIterator.

	Owned by:	Eric House

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/


#ifndef _EDITRSET_
#include "EditrSet.h"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef _ODUTILS_
#include <ODUtils.h>
#endif

#ifndef _ISOSTR_
#include "ISOStr.h"
#endif


//==============================================================================
// Class EditorSet
//==============================================================================

EditorSet::EditorSet()
{
	fSet = kODNULL;
	fEv = kODNULL;
}

void EditorSet::InitEditorSet()
{
    fEv = somGetGlobalEnvironment ();
	fSet = new OrderedCollection();
}

EditorSet::~EditorSet()
{
#ifdef OD_BUG
    if (fSet != kODNULL) {
#endif
	fSet->DeleteAll();
	delete fSet;
#ifdef OD_BUG
    }
#endif
}
	
void EditorSet::AddEditor(ODEditor editor)
{
	if ( !this->ContainsEditor(editor) )
	{
		ODULong strLength = ODISOStrLength((const ODISOStr)editor);
		ElementType newEditor = ODNewPtrClear(strLength+1,0);
		ODISOStrNCopy((const ODISOStr)newEditor, (ODISOStr)editor, strLength );
		fSet->AddLast(newEditor);
	}
}

void EditorSet::AddEditorSet( EditorSet* editors )
{
	EditorSetIterator* esi = editors->CreateIterator() ;
	for ( ODEditor editor = esi->First() ; esi->IsNotComplete() ;
			editor = esi->Next() )
	{
		this->AddEditor( editor ) ;
	}
	ODDeleteObject( esi );
}

#ifndef OD_BUG  // Method doesn't work, and no one in DR4 uses it.
void EditorSet::RemoveEditor(ODEditor editor)
{
	if (fSet->Contains((ElementType)editor) != kODFalse)
		fSet->Remove((ElementType)editor);
}
	
void EditorSet::RemoveEditor(EditorSet* editors )
{
	EditorSetIterator* esi = editors->CreateIterator() ;
	for ( ODEditor editor = esi->First() ; esi->IsNotComplete() ;
			editor = esi->Next() )
	{
		this->RemoveEditor(editor);
	}
	ODDeleteObject( esi );
}
#endif

void EditorSet::RemoveAllEditors()
{
	if ( fSet->Count() > 0 )
	{
		fSet->DeleteAll();
	}
}

ODBoolean EditorSet::ContainsEditor(ODEditor editor)
{
	ODBoolean rslt = kODFalse;
	
	if ( this->GetEditorCount() )
	{
		EditorSetIterator* editorIter = this->CreateIterator();
		
		for (ODEditor anEditor = editorIter->First();
			editorIter->IsNotComplete() && (rslt == kODFalse);
			anEditor = editorIter->Next())
		{
			rslt = (anEditor == editor) || ODISOStrEqual(anEditor, editor);
		}

		delete editorIter;
	}
	
	return rslt;
}

ODULong EditorSet::GetEditorCount()
{
	return (fSet->Count());
}

EditorSetIterator* EditorSet::CreateIterator()
{
	//return new EditorSetIterator(this);
	return new EditorSetIterator(this);
}

//==============================================================================
// Class EditorSetIterator
//==============================================================================

EditorSetIterator::EditorSetIterator(EditorSet* set)
{
	fIterator = new OrderedCollectionIterator(set->fSet);
}

EditorSetIterator::~EditorSetIterator()
{
	delete fIterator;
}
	
ODEditor EditorSetIterator::First()
{
	return (ODEditor)fIterator->First();
}

ODEditor EditorSetIterator::Next()
{
	return (ODEditor)fIterator->Next();
}

ODBoolean EditorSetIterator::IsNotComplete()
{
	return fIterator->IsNotComplete();
}
