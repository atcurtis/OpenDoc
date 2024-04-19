//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   EditorSet
//		EditorSetIterator
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
// @(#) 1.4 com/src/utils/include/EditrSet.h, odutils, od96os2, odos29646d 7/15/96 18:00:13 [ 11/15/96 15:28:59 ]
/*
	File:		EditrSet.h

	Contains:	Class definition for EditerSet and ODEditerSetIterator

	Owned by:	Eric House 

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _EDITRSET_
#define _EDITRSET_

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODOBJ_
#include "ODObject.xh"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif


//=====================================================================================
// Classes used by this interface
//=====================================================================================

class EditorSetIterator ;
class OrderedCollection;
class OrderedCollectionIterator;

//=====================================================================================
// Class EditorSet
//=====================================================================================

class EditorSet
{
	
public:

	EditorSet();
	void InitEditorSet();
	virtual ~EditorSet();

	ODMethod void AddEditor(ODEditor editor);
	
		// Adds an type to the end of the set, creating a copy of the argument in the set.

	ODMethod void AddEditorSet(EditorSet* editors);
	
		// Unions the two into this (leaving the argument unchanged).

#ifndef OD_BUG  // Method doesn't work, and no one in DR4 uses it.
	ODMethod void RemoveEditor(ODEditor editor);
	
		// Removes an editor from a set.  Does nothing if the argument editor is not present.
	
	ODMethod void RemoveEditor( EditorSet* editors );
	
		// Removes all editors present in the argument set.
#endif

	ODMethod void RemoveAllEditors();
	
		// Removes all editors from the set.

	ODMethod ODBoolean ContainsEditor(ODEditor editor);
	
		// Returns true if the set contains the specified editor.
		// Otherwise, returns false.

	ODMethod ODULong GetEditorCount();
		
	ODMethod EditorSetIterator* CreateIterator();
	
		// Returns an iterator for the set, which can be used to retrieve editors
		// from the set in order.

private:

	OrderedCollection*	fSet;

	Environment*	fEv;

	friend class EditorSetIterator;
};
	
//=====================================================================================
// Class EditorSetIterator
//=====================================================================================

class EditorSetIterator
{
	

	public:
		EditorSetIterator(EditorSet* set);
		virtual ~EditorSetIterator();

		ODVMethod ODEditor	First();
		ODVMethod ODEditor	Next();
		ODVMethod ODBoolean	IsNotComplete();
		
			// These methods return a pointer to a string within the set.
			// The client must not dispose this memory, and must be aware that
			// These methods return nil if the requested item does not exist.
		
	private:
		OrderedCollectionIterator*	fIterator;
};


#endif
