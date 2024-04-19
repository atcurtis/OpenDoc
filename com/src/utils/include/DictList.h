//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   DictionaryList
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
// @(#) 1.3 com/src/utils/include/DictList.h, odutils, od96os2, odos29646d 7/15/96 17:59:59 [ 11/15/96 15:28:58 ]
/*
	File:		DictList.h

	Contains:	Definition of class DictionaryList

	Owned by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.
	
*/

#ifndef _DICTLIST_
#define _DICTLIST_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

//==============================================================================
// Theory of Operation
//==============================================================================

// OrdereCollection is an ordered collection of elements of type void* (since
// we can't use templates)
// Duplicates are allowed.

//==============================================================================
// Constants
//==============================================================================

//==============================================================================
// Scalar Types
//==============================================================================

typedef void* KeyType;
typedef void* ValueType;

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class DictionaryList;	

//=====================================================================================
// Classes used by this interface
//=====================================================================================

class Association; // Defined in implementation

//=====================================================================================
// Global Variables
//=====================================================================================


//=====================================================================================
// Class DictionaryList
//=====================================================================================

class DictionaryList
{
	

public:

	DictionaryList();
	virtual ~DictionaryList();

	ODVMethod void	AddPair(KeyType key, ValueType value);
	
	ODVMethod ODULong Count();
	ODVMethod ODBoolean ContainsKey(KeyType key);
	ODVMethod ValueType ValueAtKey(KeyType key);

	ODVMethod void	RemoveKey(KeyType key);
	ODVMethod void	RemoveAll();			
	ODVMethod void	DeleteKeys();
	ODVMethod void	DeleteValues();
	ODVMethod void	DeleteSOMValues();
	
protected:

	 ODVMethod Association* CreateNewAssociation(KeyType key, ValueType value) const;
	 ODVMethod ODBoolean	KeysMatch(KeyType key1,KeyType key2) const;
	 
	 	// Does a straight == comparison by default 

private:

	LinkedList		fImplementation;
};

#endif // _DICTLIST_
