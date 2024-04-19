//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   StringHashTable
//		StringHashTableIterator
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
// @(#) 1.3 com/src/utils/include/StrHshTb.h, odutils, od96os2, odos29646d 7/15/96 18:02:03 [ 11/15/96 15:29:35 ]
/*
	File:		StrHshTb.h

	Contains:	Interface to StringHashTable class.

	Owned by:	Nick Pilch

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
*/

#ifndef _STRHSHTB_
#define _STRHSHTB_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

#ifndef __STDIO__
#include <stdio.h>
#endif

//==============================================================================
// Theory of Operation
//==============================================================================

/*
	A simple hash table for keys of variable length. Value is 8 bytes. First
	four bytes are a pointer to the storage for the value, the next four bytes
	are the length of that storage. The caller must allocate the storage.
	Implemented with chaining. Keys are bytes streams. Embedded NULLs are
	allowed. However, keys of zero length are not allowed.
*/

//==============================================================================
// Scalar Types
//==============================================================================

typedef ODULong (*StringHashTableFunc)(ODUByte* string,
											ODULong stringLength,
											ODULong& hashValueLowerBounds,
											ODULong& hashValueUpperBounds);

	// this is the hash function prototype.

//==============================================================================
// Classes defined in this interface
//==============================================================================

class	StringHashTable;
class	StringHashTableIterator;

//==============================================================================
// Classes used by this interface
//==============================================================================

class	LinkedNodesIterator;
struct	HashEntryRec;
struct	EntryNode;

//==============================================================================
// StringHashTable
//==============================================================================

class StringHashTable
{
	
	friend class LinkedNodesIterator;
	friend class StringHashTableIterator;

  public:

	StringHashTable(ODULong numEntries);
		// numEntries should be the expected number of table entries. The table
		//	will not grow or shrink. If zero is passed, the table is initialized
		//	with 1 entry. (not very interesting).

	ODVMethod void Initialize(ODMemoryHeapID heapID);
		// kODErrOutOfMemory will be thrown if the table could not be created.

	virtual ~StringHashTable();


	ODVMethod void			Insert(ODUByte* string, ODULong stringLength,
										ODPtr value, ODULong valueLength);
		// If this key already exists, that value is replaced by the new value.
		// 	kODErrOutOfMemory will be thrown if the entry could not be inserted.
		//	kODErrInvalidKey will be thrown if stringLength is zero.
		//	The key is copied into the table, the value is not.
		//	If an exception is 

	ODVMethod void			Remove(ODUByte* string, ODULong stringLength);
		// No exception is thrown if the key does not exist.
		//	kODErrInvalidKey will be thrown if stringLength is zero.

	ODVMethod ODBoolean 		Find(ODUByte* string, ODULong stringLength,
										ODPtr* value, ODULong* valueLength);	
		// kODTrue is returned if the value is found, kODFalse otherwise.
		//	If the entry is not found. *value and *valueLength are not
		//	guaranteed to contain any useful information.
		//	kODErrInvalidKey will be thrown if stringLength is zero.

	ODVMethod ODBoolean 		Exists(ODUByte* string, ODULong stringLength);	
		// kODTrue is returned if the value is found, kODFalse otherwise.
		//	kODErrInvalidKey will be thrown if stringLength is zero.

	ODVMethod ODULong 		GetNumEntries();	
		// Return the number of entries.

	ODVMethod void		PrintTable(FILE* outfile);
	ODVMethod void		PrintDistribution(FILE* outfile);
	
		// for debugging / testing

	StringHashTableFunc	GetHashFunc();
	void					SetHashFunc(StringHashTableFunc func);

		// get and set the hash function.

  protected:
  	ODMemoryHeapID 		fHeapID;

	HashEntryRec*	fTable; // pointer to start of table,
							//	fNumSlots * sizeof(HashEntry) long. Initialized to
							//	all zeroes.

	ODULong		fNumSlots; // Number of slots in the table, not number of
								//	entries.

	ODULong		fNumEntries;	// Actual number of entries.

	StringHashTableFunc	fHashFunc;
	
	ODVMethod HashEntryRec*	GetTable();
	ODVMethod ODULong			GetNumSlots();

	static ODULong		StdHash(ODUByte* string, ODULong stringLength,
											ODULong& hashValueLowerBounds,
											ODULong& hashValueUpperBounds);

	ODVMethod ODULong	MapToTableIndex(ODULong hash,
											ODULong hashValueLowerBounds,
											ODULong hashValueUpperBounds);

	ODVMethod ODULong	HashAndMap(ODUByte* string, ODULong stringLength);
	ODVMethod void		InsertAtIndex(ODULong index, ODUByte* string,
												ODULong stringLength,
												ODPtr value,
												ODULong valueLength);
	ODVMethod void		RemoveAtIndex(ODULong index, ODUByte* string,
												ODULong stringLength);
	EntryNode*			MakeNewNode(ODUByte* string, ODULong stringLength,
										ODPtr value, ODULong valueLength);
	ODUByte*			MakeStringFromBytes(ODUByte* string,
													ODULong stringLength);
};

//==============================================================================
// StringHashTableIterator
//
//	This iterator is only meant to be used in the the context of a for loop,
//	e.g.:
//
//	StringHashTableIterator iter;
//	for (iter.First(string, len, value, valueLen);
//			iter.IsNotComplete();
//			iter.Next(string, len, value, valueLen))
//	{
//		...
//	}
//
//==============================================================================

class StringHashTableIterator
{
  public:
	StringHashTableIterator(StringHashTable* tb);
	~StringHashTableIterator();
	void		First(ODUByte** string, ODULong* len, ODPtr* value,
						  ODULong* valueLen);
		// A pointer to the private copy of the string is returned. The user
		//	must copy the storage if he or she desires to change it.
		//	If there are no entries in the table, *string will be set to
		//	kODNULL and *len set to 0.
	void		Next(ODUByte** string, ODULong* len, ODPtr* value,
						ODULong* valueLen);
		// A pointer to the private copy of the string is returned. The user
		//	must copy the storage if he or she desires to change it.
		//	If there are no more entries in the table, *string will be set to
		//	kODNULL and *len set to 0.
	ODBoolean	IsNotComplete();
  private:
	ODBoolean	GetNext(ODUByte** string, ODULong* len,
						ODPtr* value, ODULong* valueLen);
	StringHashTable*	fTable;
	ODULong			fTableIndex;
	EntryNode*			fCurNode;
	ODBoolean			fDone;
};

#endif // _STRHSHTB_
