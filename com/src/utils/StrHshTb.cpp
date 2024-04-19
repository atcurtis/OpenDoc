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
// @(#) 1.5 com/src/utils/StrHshTb.cpp, odutils, od96os2, odos29646d 7/15/96 17:59:34 [ 11/15/96 15:28:56 ]
/*
	File:		StrHshTb.cpp

	Contains:	Implementation for StringHashTable class.

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

*/

/*
	A simple hash table with chaining. Each hash table entry
	is a HashEntryRec. Each HashEntryRec contains a pointer to start of a
	linked list of EntryNodes. Each EntryNode contains the key, the length of
	key, the value, the length of the value and at least
	one link to another item in the list.
	
	The keys are stored as NULL terminated C-strings for debugging purposes
	even though they may have imbedded NULLs.
*/

#ifndef _STRHSHTB_
#include "StrHshTb.h"
#endif

#ifndef __CTYPE__
#include <ctype.h>
#endif

#ifndef __LIMITS__
#include <limits.h>
#endif

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef __STRING__
#include <string.h>
#endif

#ifndef _ODNEW_
#include "ODNew.h"
#endif

#if !defined(_PLATFORM_MACINTOSH_) && !defined(_ODTYPESP_)
#include <ODTypesP.h>
#endif

#ifndef _UTILERRS_
#include "UtilErrs.h"
#endif

#pragma segment StringHashTable

//==============================================================================
// Local Classes
//==============================================================================

struct EntryNode
{
	ODUByte*	key;
	ODULong	keySize;
	ODPtr		value;
	ODULong	valueLen;
	EntryNode*	nextEntry; // == 0 means end of list.
};

struct HashEntryRec
{
	EntryNode*	node;		//	pointer to first node
};

#if 0
	//OBSOLETE
	struct LinkedNodesIterator
	{
		LinkedNodesIterator(StringHashTable* table, ODULong tableIndex)
							{fTableIndex = tableIndex; fTable = table;
								fCurNode = (table->fTable + tableIndex)->node;}
		EntryNode*	First() {return fCurNode;}
		EntryNode*	Next() {return (fCurNode = fCurNode->nextEntry);}
		ODBoolean	IsNotComplete() {return (fCurNode->nextEntry != 0);}
	  private:
		StringHashTable*	fTable;
		ODULong			fTableIndex;
		EntryNode*			fCurNode;
	};
#endif // 0

struct LinkedNodesIterator
{
  public:
	LinkedNodesIterator(StringHashTable* table, ODULong tableIndex);
	EntryNode*	First();
	EntryNode*	Next();
	ODBoolean	IsNotComplete();
  private:
	StringHashTable*	fTable;
	ODULong			fTableIndex;
	EntryNode*			fCurNode;
	ODBoolean			fDone;
};

LinkedNodesIterator::LinkedNodesIterator(StringHashTable* table, ODULong tableIndex)
{
	fTableIndex = tableIndex;
	fTable = table;
	fCurNode = (table->GetTable() + tableIndex)->node;
	fDone = kODFalse;
}

EntryNode*	LinkedNodesIterator::First()
{
	return fCurNode;
}

EntryNode*	LinkedNodesIterator::Next()
{
	if (fCurNode->nextEntry == kODNULL)
	{
		fDone = kODTrue;
		return 0;
	}
	else
		return (fCurNode = fCurNode->nextEntry);
}

ODBoolean	LinkedNodesIterator::IsNotComplete()
{
	if (!fCurNode)
		return kODFalse;
	else
		return !fDone;
}


//==============================================================================
// StringHashTable
//==============================================================================

//------------------------------------------------------------------------------
// StringHashTable::StringHashTable
//------------------------------------------------------------------------------

StringHashTable::StringHashTable(ODULong numEntries)
{
	if (numEntries == 0)
		fNumSlots = 1;
	else
		fNumSlots = numEntries;
	fHashFunc = StringHashTable::StdHash;
	fNumEntries = 0;
	fHeapID = kDefaultHeapID;
}

//------------------------------------------------------------------------------
// StringHashTable::Initialize
//------------------------------------------------------------------------------

void StringHashTable::Initialize(ODMemoryHeapID heapID)
{
	ODULong	tableSize = fNumSlots * sizeof(HashEntryRec);

	fHeapID = heapID;
	fTable = (HashEntryRec*)ODNewPtrClear(tableSize, fHeapID);
	if (!fTable)
		THROW(kODErrOutOfMemory);
}

//------------------------------------------------------------------------------
// StringHashTable::~StringHashTable
//------------------------------------------------------------------------------

StringHashTable::~StringHashTable()
{
	// gotta be careful to delete a node only after we're done with it.
	for (ODULong i = 0; i < fNumSlots; i++)
	{
		LinkedNodesIterator	iter(this, i);
		EntryNode*			someNode;
		EntryNode*			lastNode = 0; // first time through

		for (someNode = iter.First();
				iter.IsNotComplete();
				someNode = iter.Next())
		{
			if (lastNode)
			{
				ODDisposePtr((Ptr)lastNode->key);
				ODDisposePtr((Ptr)lastNode);
			}
			lastNode = someNode;
		}
		if (lastNode)
		{
			ODDisposePtr((Ptr)lastNode->key);
			ODDisposePtr((Ptr)lastNode);
		}
	}
	ODDisposePtr((Ptr)fTable);
}

//------------------------------------------------------------------------------
// StringHashTable::Insert
//
//	Hash the key, Map the hash to the range of table index values, insert the
//	hash into the linked list of EntryNodes at that table slot.
//------------------------------------------------------------------------------

void StringHashTable::Insert(ODUByte* string, ODULong stringLength,
										ODPtr value, ODULong valueLength)
{
	if (stringLength == 0)
		THROW(kODErrInvalidKey);
	if (value == (ODPtr)kODNULL)
		THROW(kODErrIllegalNullInput);
	ODULong tableIndex = this->HashAndMap(string, stringLength);
	this->InsertAtIndex(tableIndex, string, stringLength, value, valueLength);
	++fNumEntries;
}

//------------------------------------------------------------------------------
// StringHashTable::InsertAtIndex
//
//	Insert a hash table node at the the specified index in the table.
//------------------------------------------------------------------------------

void StringHashTable::InsertAtIndex(ODULong index, ODUByte* string,
												ODULong stringLength,
												ODPtr value,
												ODULong valueLength)
{
	HashEntryRec*	entry = fTable + index;
	
	// No nodes at this table entry? Stick in the first one.
	if (entry->node == kODNULL)
		entry->node = this->MakeNewNode(string, stringLength, value,
										valueLength);
	// Else, need to follow chain to end checking for duplicate keys
	else
	{
		LinkedNodesIterator	iter(this, index);
		EntryNode*			someNode;
		EntryNode*			lastNode;
		ODBoolean			replaceOne = kODFalse;
		ODBoolean			alreadyHadKey = kODFalse;

		// Iterate to the end of the list or until we find the key
		for (someNode = iter.First();
				iter.IsNotComplete();
				lastNode = someNode, someNode = iter.Next())
		{
			if ((someNode->keySize == stringLength)
				&& (!memcmp((const char*)someNode->key, (const char*)string,
							(size_t)stringLength)))
			{
				someNode->value = value;
				someNode->valueLen = valueLength;
				alreadyHadKey = kODTrue;
				break;
			}
		}
		if (!alreadyHadKey)
			// If we got here, we have to create a new node.
			lastNode->nextEntry = MakeNewNode(string, stringLength, value,
												valueLength);
	}
}

//------------------------------------------------------------------------------
// StringHashTable::MakeStringFromBytes
//
//	Create a new NULL-terminated C-string from a stream of bytes.
//------------------------------------------------------------------------------

ODUByte* StringHashTable::MakeStringFromBytes(ODUByte* string,
													ODULong stringLength)
{
	ODUByte* newString = (ODUByte*)ODNewPtr(stringLength + 1, fHeapID);
	if (!newString)
		THROW(kODErrOutOfMemory);
	ODBlockMove(string, newString, stringLength);
	newString[stringLength] = 0;
	return newString;
}

//------------------------------------------------------------------------------
// StringHashTable::MakeNewNode
//
//	Allocate and intialize a new entry node.
//------------------------------------------------------------------------------

EntryNode* StringHashTable::MakeNewNode(ODUByte* string,
											ODULong stringLength, ODPtr value,
											ODULong valueLength)
{
	EntryNode* newNode = (EntryNode*)ODNewPtr(sizeof(EntryNode), fHeapID);

	// Convert strings to C-strings and copy
	ODUByte* newString = this->MakeStringFromBytes(string, stringLength);
	
	newNode->key = newString;
	newNode->keySize = stringLength;
	newNode->nextEntry = kODNULL;
	newNode->value = value;
	newNode->valueLen = valueLength;
	
	return newNode;
}

//------------------------------------------------------------------------------
// StringHashTable::Remove
//
//	Hash the key, Map the hash to the range of table index values, search the
//	the linked list of EntryNodes for the key, remove the EntryNode in which
//	the key was found.
//------------------------------------------------------------------------------

void StringHashTable::Remove(ODUByte* string, ODULong stringLength)
{
	if (stringLength == 0)
		THROW(kODErrInvalidKey);
	ODULong tableIndex = this->HashAndMap(string, stringLength);
	this->RemoveAtIndex(tableIndex, string, stringLength);
	--fNumEntries;
}

//------------------------------------------------------------------------------
// StringHashTable::RemoveAtIndex
//
//	Remove a hash table node at the the specified index in the table.
//------------------------------------------------------------------------------

void StringHashTable::RemoveAtIndex(ODULong index, ODUByte* string,
												ODULong stringLength)
{
	HashEntryRec*	entry = fTable + index;
	
	if (entry->node != kODNULL)
	{
		LinkedNodesIterator	iter(this, index);
		EntryNode*			someNode;
		EntryNode*			prevNode = 0; // start of chain

		for (someNode = iter.First();
				iter.IsNotComplete();
				someNode = iter.Next())
		{
			// look for a match
			if ((someNode->keySize == stringLength)
				&& (!memcmp((const char*)someNode->key, (const char*)string,
							(size_t)stringLength)))
			{
				// Special case first node
				if (prevNode == 0)
					entry->node = someNode->nextEntry;
				else
					prevNode->nextEntry = someNode->nextEntry;

				// Must dispose someNode->key first or else we'll lose its
				//	reference after disposing the someNode.
				ODDisposePtr((Ptr)someNode->key);
				ODDisposePtr((Ptr)someNode);
				break;
			}
			prevNode = someNode;
		}
	}
}

//------------------------------------------------------------------------------
// StringHashTable::Find
//
//	Hash the key, Map the hash to the range of table index values, search the
//	the linked list of EntryNodes for the key. Signal error if not found.
//------------------------------------------------------------------------------

ODBoolean StringHashTable::Find(ODUByte* string, ODULong stringLength,
									ODPtr* value, ODULong* valueLength)
{
	ODULong		tableIndex = this->HashAndMap(string, stringLength);
	HashEntryRec*	entry = fTable + tableIndex;
	ODBoolean		result = kODFalse;
	
	if (stringLength == 0)
		THROW(kODErrInvalidKey);

	if (entry->node != kODNULL)
	{
		LinkedNodesIterator	iter(this, tableIndex);
		EntryNode*			someNode;

		for (someNode = iter.First();
				iter.IsNotComplete();
				someNode = iter.Next())
		{
			// match
			if ((someNode->keySize == stringLength)
				&& (!memcmp((const char*)someNode->key, (const char*)string,
							(size_t)stringLength)))
			{
				*value = someNode->value;
				*valueLength = someNode->valueLen;
				result = kODTrue;
				break;
			}
		}
	}

	return result;
}

//------------------------------------------------------------------------------
// StringHashTable::Exists
//
//	Hash the key, Map the hash to the range of table index values, search the
//	the linked list of EntryNodes for the key. Signal error if not found.
//------------------------------------------------------------------------------

ODBoolean StringHashTable::Exists(ODUByte* string, ODULong stringLength)
{
	ODULong		tableIndex = this->HashAndMap(string, stringLength);
	HashEntryRec*	entry = fTable + tableIndex;
	ODBoolean		result = kODFalse;
	
	//if (stringLength == 0)
		//THROW(kODErrInvalidKey);

	if (stringLength != 0)
	{
		if (entry->node != kODNULL)
		{
			LinkedNodesIterator	iter(this, tableIndex);
			EntryNode*			someNode;
	
			for (someNode = iter.First();
					iter.IsNotComplete();
					someNode = iter.Next())
			{
				// match
				if ((someNode->keySize == stringLength)
					&& (!memcmp((const char*)someNode->key, (const char*)string,
								(size_t)stringLength)))
				{
					result = kODTrue;
					break;
				}
			}
		}
	}

	return result;
}

//------------------------------------------------------------------------------
// StringHashTable::GetNumEntries
//------------------------------------------------------------------------------

ODULong StringHashTable::GetNumEntries()
{
	return fNumEntries;
}

//------------------------------------------------------------------------------
// StringHashTable::StdHash
//
//	Adds all the characters in the string and returns the smallest and largest
//	numbers that they could add up to in hashValueLowerBounds and
//	hashValueUpperBounds.
//------------------------------------------------------------------------------

ODULong StringHashTable::StdHash(ODUByte* string,
											ODULong stringLength,
											ODULong& hashValueLowerBounds,
											ODULong& hashValueUpperBounds)
{
	ODULong	result = 0;

	for (ODULong i = 0; i < stringLength; i++)
	{
		result += *(string + i);
	}
	hashValueLowerBounds = 0;
	hashValueUpperBounds = 255 * stringLength;
	return result;
}

//------------------------------------------------------------------------------
// StringHashTable::MapToTableIndex
//
//	Map hash value in its range to the range 0..(fNumSlots - 1).
//------------------------------------------------------------------------------

ODULong StringHashTable::MapToTableIndex(ODULong hash,
												ODULong hashValueLowerBounds,
												ODULong hashValueUpperBounds)
{
	if (hashValueLowerBounds >= hashValueUpperBounds)
		return 0;
	else
	{
		double index = (double)(hash - hashValueLowerBounds)
			/ (double)(hashValueUpperBounds - hashValueLowerBounds)
			* (double)(fNumSlots - 1);
		double indexToRound = index + .5;
		return (ODULong)indexToRound;
	}
}

//------------------------------------------------------------------------------
// StringHashTable::HashAndMap
//
//	Utility function.
//------------------------------------------------------------------------------

ODULong StringHashTable::HashAndMap(ODUByte* string, ODULong stringLength)
{
	ODULong	hashUpperBounds, hashLowerBounds;

	ODULong hash = fHashFunc(string, stringLength, hashLowerBounds,
								hashUpperBounds);
	return this->MapToTableIndex(hash, hashLowerBounds, hashUpperBounds);
}

//------------------------------------------------------------------------------
// StringHashTable::PrintTable
//
//	Testing / Debugging function.
//------------------------------------------------------------------------------

#include <stdio.h>

void StringHashTable::PrintTable(FILE* outfile)
{
	StringHashTableIterator	iter(this);
	ODUByte*					string;
	ODULong					len;
	ODPtr						value;
	ODULong					valueLen;

	fprintf(outfile, "The table contains this:\n");

	for (iter.First(&string, &len, &value, &valueLen);
			iter.IsNotComplete();
			iter.Next(&string, &len, &value, &valueLen))
	{	
		fprintf(outfile, "%s, %d%d\n", string, value, valueLen);
	}

	fprintf(outfile, "\n");
}

//------------------------------------------------------------------------------
// StringHashTable::PrintDistribution
//
//	Testing / Debugging function.
//------------------------------------------------------------------------------

void StringHashTable::PrintDistribution(FILE* outfile)
{
	fprintf(outfile, "The table looks like this:\n");

	for (ODULong i = 0; i < fNumSlots; i++)
	{
		fprintf(outfile, "Slot %d:\n", i);

		LinkedNodesIterator	iter(this, i);
		EntryNode*			someNode;

		for (someNode = iter.First();
				iter.IsNotComplete();
				someNode = iter.Next())
		{
			fprintf(outfile, "\t%s\n", someNode->key);
		}
	}

	fprintf(outfile, "\n");
}

//------------------------------------------------------------------------------
// StringHashTable::GetHashFunc
//------------------------------------------------------------------------------

StringHashTableFunc StringHashTable::GetHashFunc()
{
	return fHashFunc;
}

//------------------------------------------------------------------------------
// StringHashTable::SetHashFunc
//------------------------------------------------------------------------------

void StringHashTable::SetHashFunc(StringHashTableFunc func)
{
	fHashFunc = func;
}

//------------------------------------------------------------------------------
// StringHashTable::GetNumSlots
//
//	For friends.
//------------------------------------------------------------------------------

ODULong StringHashTable::GetNumSlots()
{
	return fNumSlots;
}

//------------------------------------------------------------------------------
// StringHashTable::GetTable
//
//	For friends.
//------------------------------------------------------------------------------

HashEntryRec* StringHashTable::GetTable()
{
	return fTable;
}

//==============================================================================
// StringHashTableIterator
//==============================================================================

//------------------------------------------------------------------------------
// StringHashTableIterator::StringHashTableIterator
//------------------------------------------------------------------------------

StringHashTableIterator::StringHashTableIterator(StringHashTable* tb)
{
	fTable = tb;
	fTableIndex = 0;
	fCurNode = kODNULL;
	fDone = kODFalse;
}

//------------------------------------------------------------------------------
// StringHashTableIterator::~StringHashTableIterator
//------------------------------------------------------------------------------

StringHashTableIterator::~StringHashTableIterator()
{
}

//------------------------------------------------------------------------------
// StringHashTableIterator::First
//
//	Use GetNext to find first. Set flag if we don't find an entry.
//------------------------------------------------------------------------------

void StringHashTableIterator::First(ODUByte** string, ODULong* len,
											ODPtr* value,
											ODULong* valueLength)
{
	if (!this->GetNext(string, len, value, valueLength))
	{
//		*len  = 0;
//		*string = (ODUByte*)kODNULL;
		fDone = kODTrue;
	}
}

//------------------------------------------------------------------------------
// StringHashTableIterator::Next
//
//	Use GetNext. Set flag if we don't find an entry.
//------------------------------------------------------------------------------

void StringHashTableIterator::Next(ODUByte** string, ODULong* len,
											ODPtr* value,
											ODULong* valueLength)
{
	if (!this->GetNext(string, len, value, valueLength))
	{
//		*len  = 0;
//		*string = (ODUByte*)kODNULL;
		fDone = kODTrue;
	}
}

//------------------------------------------------------------------------------
// StringHashTableIterator::IsNotComplete
//
//	Return value of previously set flag.
//------------------------------------------------------------------------------

ODBoolean StringHashTableIterator::IsNotComplete()
{
	return !fDone;
}

//------------------------------------------------------------------------------
// StringHashTableIterator::GetNext
//
// kODTrue is returned if an entry was found, kODFalse otherwise. The
//	values pointed to by the parameters are undefined if kODFalse is
//	returned.
//	We can record the index of the last hash bucket we were looking in, but we
//	can't record the index into the linked list at that bucket. So we keep a
//	pointer to the last node found and look for it the next time we iterate
//	through that list.
//------------------------------------------------------------------------------

ODBoolean StringHashTableIterator::GetNext(ODUByte** string, ODULong* len,
												ODPtr* value,
												ODULong* valueLength)
{
	for (ODULong i = fTableIndex; i < fTable->GetNumSlots(); i++)
	{
		LinkedNodesIterator	iter(fTable, i);
		EntryNode*			someNode;
		ODBoolean			pastLastNode = fCurNode ? kODFalse : kODTrue;

		for (someNode = iter.First();
				iter.IsNotComplete();
				someNode = iter.Next())
		{
			if ((i == fTableIndex) && (someNode == fCurNode))
			{
				pastLastNode = kODTrue;
				continue;
			}
			if (pastLastNode || (i != fTableIndex))
			{
				fTableIndex = i;
				fCurNode = someNode;
				*string = someNode->key;
				*len = someNode->keySize;
				*value = someNode->value;
				*valueLength = someNode->valueLen;
				return kODTrue;
			}
		}
	}
	return kODFalse;
}

