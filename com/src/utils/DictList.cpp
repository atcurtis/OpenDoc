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
// @(#) 1.3 com/src/utils/DictList.cpp, odutils, od96os2, odos29646d 7/15/96 17:57:34 [ 11/15/96 15:28:42 ]
/*
	File:		DictList.cpp

	Contains:	Implementation of DictionaryList

	Written by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _DICTLIST_
#include "DictList.h"
#endif

#include <somobj.xh>

#pragma segment ODCollections

//=====================================================================================
// Class Association
//=====================================================================================

class Association : public Link {
	public:
		Association(KeyType key, ValueType value);		
		virtual ~Association();
		KeyType	GetKey()	{ return fKey;}
		void		SetKey(KeyType key)	{ fKey = key;}
		ValueType	GetValue()	{ return fValue;}
		void		SetValue(ValueType value)	{ fValue = value;}
	
	private:
		KeyType 		fKey;
		ValueType 		fValue;
};
	
Association::Association(KeyType key, ValueType value)
{
	fKey = key;
	fValue = value;
}

Association::~Association()
{
}

//======================================================================================
// Class DictionaryList
//======================================================================================

//------------------------------------------------------------------------------
// DictionaryList::DictionaryList
//
// Description
//------------------------------------------------------------------------------

DictionaryList::DictionaryList()
{
}

//------------------------------------------------------------------------------
// DictionaryList::~DictionaryList
//
// Description
//------------------------------------------------------------------------------

DictionaryList::~DictionaryList()
{
	this->RemoveAll();
}

//------------------------------------------------------------------------------
// DictionaryList::AddPair
//
// Description
//------------------------------------------------------------------------------

void DictionaryList::AddPair(KeyType key, ValueType value)
{
	ODBoolean found = kODFalse;
	LinkedListIterator iter(&fImplementation);
	Association* association = (Association*) iter.First();
	while (association != kODNULL)
	{
		KeyType k = association->GetKey();

		if (this->KeysMatch(k,key))
		{
			found = kODTrue;
			association->SetValue(value);
			break;
		}
		else
			association = (Association*) iter.Next();
	}	
	if (!found)
	{
		association = this->CreateNewAssociation(key,value);
		fImplementation.AddLast(association);
	}
}

//------------------------------------------------------------------------------
// DictionaryList::Count
//
// Description
//------------------------------------------------------------------------------

ODULong DictionaryList::Count()
{
	return fImplementation.Count();
}


//------------------------------------------------------------------------------
// DictionaryList::ContainsKey
//
// Description
//------------------------------------------------------------------------------

ODBoolean DictionaryList::ContainsKey(KeyType key)
{
	LinkedListIterator iter(&fImplementation);
	Association* association = (Association*) iter.First();
	while (association != kODNULL)
	{
		KeyType k = association->GetKey();

		if (this->KeysMatch(k,key))
		{
			return kODTrue;	
		}
		else
			association = (Association*) iter.Next();
	}	
	return kODFalse;
}

//------------------------------------------------------------------------------
// DictionaryList::ValueAtKey
//
// Description
//------------------------------------------------------------------------------

ValueType DictionaryList::ValueAtKey(KeyType key)
{
	LinkedListIterator iter(&fImplementation);
	Association* association = (Association*) iter.First();
	while (association != kODNULL)
	{
		KeyType k = association->GetKey();

		if (this->KeysMatch(k,key))
		{
			return association->GetValue();	
		}
		else
			association = (Association*) iter.Next();
	}	
	return kODNULL;
}

//------------------------------------------------------------------------------
// DictionaryList::RemoveKey
//
// Description
//------------------------------------------------------------------------------

void DictionaryList::RemoveKey(KeyType key)
{
	LinkedListIterator iter(&fImplementation);
	Association* association = (Association*) iter.First();
	while (association != kODNULL)
	{
		KeyType k = association->GetKey();

		if (this->KeysMatch(k,key))
		{
			fImplementation.Remove(*association);
			break;
		}
		else
			association = (Association*) iter.Next();
	}	
}

//------------------------------------------------------------------------------
// DictionaryList::RemoveAll
//
// Description
//------------------------------------------------------------------------------

void DictionaryList::RemoveAll()
{
	Link* link = fImplementation.RemoveFirst();
	while (link != kODNULL)
	{
		delete link;
		link = fImplementation.RemoveFirst();
	}
}

//------------------------------------------------------------------------------
// DictionaryList::DeleteKeys
//
// Description
//------------------------------------------------------------------------------

void DictionaryList::DeleteKeys()
{
	LinkedListIterator iter(&fImplementation);
	
	for (Association* association = (Association*) iter.First();
		iter.IsNotComplete();
		association = (Association*) iter.Next())
	{
		KeyType key = association->GetKey();
		delete key; 
	}
}

//------------------------------------------------------------------------------
// DictionaryList::DeleteValues
//
// Delete non-class values; plain vanilla global operator delete will be called
//------------------------------------------------------------------------------

void DictionaryList::DeleteValues()
{
	LinkedListIterator iter(&fImplementation);
	
	for (Association* association = (Association*) iter.First();
		iter.IsNotComplete();
		association = (Association*) iter.Next())
	{
		ValueType value = association->GetValue();
		delete value; 
	}
}

//------------------------------------------------------------------------------
// DictionaryList::DeleteSOMValues
//
// Delete values that are known to be SOM objects
//------------------------------------------------------------------------------

void DictionaryList::DeleteSOMValues()
{
	LinkedListIterator iter(&fImplementation);
	
	for (Association* association = (Association*) iter.First();
		iter.IsNotComplete();
		association = (Association*) iter.Next())
	{
		SOMObject *value = (SOMObject*) association->GetValue();
		if( value )
			delete value;
	}
}

//------------------------------------------------------------------------------
// DictionaryList::CreateNewAssociation
//
// Description
//------------------------------------------------------------------------------

Association*	DictionaryList::CreateNewAssociation(KeyType key, ValueType value) const
{
	return new Association(key, value);
}

//------------------------------------------------------------------------------
// DictionaryList::KeysMatch
//
// Description
//------------------------------------------------------------------------------

ODBoolean	DictionaryList::KeysMatch(KeyType key1,KeyType key2) const
{
	return (key1 == key2);
}

