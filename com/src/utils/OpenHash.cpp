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
// @(#) 1.5 com/src/utils/OpenHash.cpp, odutils, od96os2, odos29646d 7/15/96 17:59:08 [ 11/15/96 15:28:48 ]
/*
	File:		OpenHash.cpp

	Contains:	OpenHashTable, a hash table with "open" hashing

	Owned by:	David McCusker

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#include <ODPagtun.h>
#endif

#ifndef _OPENHASH_
#include "OpenHash.h"
#endif

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

//==============================================================================
// OpenHashEntry: theory of operation
//==============================================================================

/*
	OpenHashEntry is part of the the representation of associations inside
	a hash table.  The most important role that a OpenHashEntry has is
	its index relative to the beginning of a vector of OpenHashEntry's; this
	index is the same index of the key and value in separate vectors.
	Allocating a OpenHashEntry at a given index means that the corresponding
	key and value at the same index are also allocated.
*/

class OpenHashEntry {	

public: // private (to classes in this file) by convention
	OpenHashEntry*  fLink;

#ifdef OPENHASH_CACHE_HASH
	ODULong         fHash;
#endif
	
public:
	~OpenHashEntry()      {  }
	OpenHashEntry()       {  }
	
#ifdef OPENHASH_CACHE_HASH
	void InitEntry(ODULong hash) 
	                      { fHash = hash; }
	ODULong Hash() const  { return fHash; }
#endif
};

//==============================================================================
// OpenHashVectors: theory of operation
//==============================================================================

/*
	OpenHashVectors is a helper class which factors out the allocation
	of vectors for OpenHashTable.  The general intention is to reduce
	code size, by avoiding duplication in OpenHashTable::OpenHashTable(),
	OpenHashTable::ShrinkToFit() and OpenHashTable::Grow().
	
	OpenHashVectors is initialized by OpenHashTable::NewVectors().
*/

class OpenHashVectors {	

public: // private (to classes in this file) by convention
	OpenHashEntry**  fTable;       // hash buckets (numbering fSlots)
	OpenHashEntry*   fEntries;     // block of fSlots entries
	char*            fKeys;        // block of fSlots keys
	char*            fValues;      // block of fSlots values
	OpenHashSize     fSlots;       // current size (capacity) of table
	
public:
	~OpenHashVectors()      {  }
	OpenHashVectors()       {  }

	void Destroy();
};

void OpenHashVectors::Destroy()
{
	if (fTable) ODDisposePtr(fTable);
	if (fEntries) ODDisposePtr(fEntries);
	if (fKeys) ODDisposePtr(fKeys);
	if (fValues) ODDisposePtr(fValues);
}


//==============================================================================
// OpenHashTable
//==============================================================================

// OpenHashTable structure
// Here is a picture of a hash table under the following conditions:
// * OPENHASH_CACHE_HASH is not defined.
// * fSlots == 4, fCount == 3 (there is one free slot left in the table)
// * Keys are single bytes, fKeySize == 1 == sizeof(char).
// * Values are shorts (2-byte ints), fValSize == 2 == sizeof(short).
// * The hash of a key is just the byte value of the key.
// * The table contains the following associations (added in this order):
//   <(char)1, (short) 0x10>, <(char)2, (short) 0x20>, <(char)5, (short) 0x50>
// * fEntries begins at address 0x100.
// Note that keys 1 and 5 both hash to the same bucket (1 % 4) == (5 % 4).
//     
// === The PHYSICAL representation of this table is as follows: === 
//
// fTable   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
//          |  entry = 0    | entry = 0x108 | entry = 0x104 |  entry = 0    |         
//          +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
//
//           0x100:          0x104:          0x108:          0x10C:
// fEntries +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
//          |  fLink = 0    |  fLink = 0    | fLink = 0x100 |  fLink = 0    |            
//          +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
//
// fKeys    +---+---+---+---+
//          | 1 | 2 | 5 |???|            
//          +---+---+---+---+
//
// fValues  +---+---+---+---+---+---+---+---+
//          |  0x10 |  0x20 |  0x50 |???????|            
//          +---+---+---+---+---+---+---+---+
//
// fFreeList == 0x10C  // points to last fEntries cell
//
//  === The LOGICAL representation of this table is as follows: === 
//
//      +---+
//      |  /|
//   0  | / |
//      |/  |
//      +---+                108:                100:
//      |   |   +---+---+---+---+   +---+---+---+---+
//   1  | *---->| 5 |  0x50 | *---->| 1 |  0x10 | / |
//      |   |   +---+---+---+---+   +---+---+---+---+
//      +---+                104:
//      |   |   +---+---+---+---+
//   2  | *---->| 2 |  0x20 | / |
//      |   |   +---+---+---+---+
//      +---+
//      |  /|                                    10C:
//   3  | / |                       +---+---+---+---+
//      |/  |          free list -->|???|???????| / |
//      +---+                       +---+---+---+---+
//
// The fEntries slots were allocated sequentially as they were pulled off
// the free list of entries; the slots in fKeys and fValues were allocated
// in parallel with the fEntries slots (key and value allocation is by
// entry index).

OpenHashEntry** OpenHashTable::NewTable(OpenHashSize slots) const
	// Assume slots is never zero.  Make a vector of zeroed ptrs.
{
	ODBlockSize size = (ODBlockSize) (slots * sizeof(OpenHashEntry*));
	return (OpenHashEntry**) ODNewPtrClear(size, fHeap);
}

char* OpenHashTable::NewKeys(OpenHashSize slots) const
	// Assume slots is never zero.  Make a vector of zeroed space.
{
	ODBlockSize size = (ODBlockSize) (slots * fKeySize);
	return (char*) ODNewPtrClear(size, fHeap);
}

char* OpenHashTable::NewValues(OpenHashSize slots) const
	// Assume slots is never zero.  Make a vector of zeroed space.
{
	ODBlockSize size = (ODBlockSize) (slots * fValSize);
	if (size)
		return (char*) ODNewPtrClear(size, fHeap);
	else
		return (char*) 0L;
}

OpenHashEntry* OpenHashTable::NewEntries(OpenHashSize slots) const
	// Assume slots is never zero.  Allocate a vector of entries and chain
	// them together to form a linked list with the first elememt at the
	// head and the last element at the tail.  Thus, the whole vector of
	// entries forms the free list, or any number can be removed from the
	// beginning and the remainder become the free list.
{
	ODBlockSize size = (ODBlockSize) (slots * sizeof(OpenHashEntry));
	OpenHashEntry* vector = (OpenHashEntry*) ODNewPtr(size, fHeap);
	if (vector)
	{
		// Chain the entries into a list so vector is the head.
		register OpenHashEntry* v = vector + (slots - 1); // last entry
		v->fLink = 0L; // terminate the end of the list
		while (--v >= vector)
			v->fLink = v + 1;
	}
	return vector;
}

ODBoolean OpenHashTable::NewVectors(OpenHashVectors* old, OpenHashSize newSlots)
{
	old->fTable   = fTable;
	old->fEntries = fEntries;
	old->fKeys    = fKeys;
	old->fValues  = fValues;
	old->fSlots   = fSlots;
	
	OpenHashEntry** newTable  = NewTable(newSlots);
	OpenHashEntry* newEntries = NewEntries(newSlots);
	char* newKeys             = NewKeys(newSlots);
	char* newValues           = NewValues(newSlots);
	
	if (newTable && newEntries && newKeys && (newValues || !fValSize))
	{
		// do not modify old member values until all allocation succeeds:
		fTable = newTable;
		fFreeList = fEntries = newEntries;
		fKeys = newKeys;
		fValues = newValues;
		fSlots = newSlots;	
		++fSeed; // indicate hash table is modified
		return kODTrue;
	}
	else
	{
		if (newTable) ODDisposePtr(newTable);
		if (newEntries) ODDisposePtr(newEntries);
		if (newKeys) ODDisposePtr(newKeys);
		if (newValues) ODDisposePtr(newValues);
		return kODFalse;
	}
}

OpenHashEntry** OpenHashTable::Find(void* key, ODULong hash) const
{
	OpenHashEntry** where = fTable + (hash % fSlots);	
	OpenHashEntry* e = *where;	
	while ( e )
	{
		ODULong i = e - fEntries;
#ifdef OPENHASH_CACHE_HASH
		if (e->Hash() == hash && (*fEqual)(fKeys + (i * fKeySize), key))
			return where;
#else
		if ((*fEqual)(fKeys + (i * fKeySize), key))
			return where;
#endif
		where = &e->fLink;
		e = *where;
	}
	return (OpenHashEntry**) 0L;
}

inline OpenHashEntry* OpenHashTable::PopFreeEntry()
{
	OpenHashEntry* e = fFreeList;
	if (e)
		fFreeList = e->fLink;
	return e;
}

inline void OpenHashTable::PushFreeEntry(OpenHashEntry* e)
{
	e->fLink = fFreeList;
	fFreeList = e;
}

void OpenHashTable::Clear()
{
	if (fTable)
	{
		ODDisposePtr(fTable);
		fTable = 0;
	}
	if (fEntries)
	{
		ODDisposePtr(fEntries);
		fEntries = 0;
	}
	if (fKeys)
	{
		ODDisposePtr(fKeys);
		fKeys = 0;
	}
	if (fValues)
	{
		ODDisposePtr(fValues);
		fValues = 0;
	}
	fCount = 0;
}

OpenHashTable::~OpenHashTable()
{
	Clear();
}

ODBoolean OpenHashTable::StdEqual(const void* key, const void* testKey)
	// A standard equal function useable when keys are pointers or ints.
	// Equality is defined as pointer equality.  Note that since
	// a pointer to the key is passed, the argument type is actually
	// pointer to a pointer.
	// returns ( *(void**) key == *(void**) testKey);
{
	return (*(void**) key == *(void**) testKey);
}
	
ODBoolean OpenHashTable::EqualTwoLongs(const void* key, const void* testKey)
	// A standard equal function for an 8-byte key. Compares the
	// first two longwords of each keys as unsigned longs.
{
	return (*(ODULong*) key == *(ODULong*) testKey) && 
		(((ODULong*) key)[1] == ((ODULong*) testKey)[1]);
}

ODULong OpenHashTable::StdHash(const void* key)
	// A standard hash function useable when keys are pointers or ints.
	// Hash is defined as the pointer cast to an integer.  Note that since
	// a pointer to the key is passed, the argument type is actually
	// pointer to a pointer.
	// returns *(ODULong*) key;
{
	return *(ODULong*) key;
}

// some constants for HashLong:
enum {
	OpenHashTable_kMersennePrime = 2147483647,  // 0x7FFFFFFF
	OpenHashTable_kMult = 48271L,	    // 48271 is a prime number
	OpenHashTable_kQuotient = 44488L,   // quo is mersenne-prime / mult
	OpenHashTable_kRem = 3399L          // rem is mersenne-prime % mult
};

ODULong OpenHashTable::Random(register ODULong seed)
	// Pseudo-random number generator with period 2**31-1 (i.e. all
	// values in 1..2**31-1 are visted before repetition). Useful
	// for hashing one or more longwords. Derived from published
	// literature on good random number generators (the names
	// Park and Miller come to mind).
{
	// The idea is to multiply by the multiplier and then mod by
	// mersenne-prime, but in order to avoid 32-bit overflow, a tricky
	// but arithmetically equivalent procedure must be used.  If you
	// allow 32-bit overflow to occur, you kill the nice period as well as
	// other nice attributes of this generator.
	
	if (seed) // avoid generating zero
	{
		long high = seed / OpenHashTable_kQuotient;
		long low = seed % OpenHashTable_kQuotient;
		register long test = 
			(OpenHashTable_kMult * low) - (OpenHashTable_kRem * high);
		seed = (test > 0)?	test : test + OpenHashTable_kMersennePrime;
	}
	else
		seed = 1;
	return seed;
}

ODULong OpenHashTable::RandomOneLong(const void* key)
	// A standard hash function for a 4-byte key, based on Random().
	// returns Random( *(ODULong*) key );
{
	return Random( *(ODULong*) key );
}

ODULong OpenHashTable::RandomTwoLongs(const void* key)
	// A standard hash function for an 8-byte key, based on Random().
	// returns Random( ((ODULong*) key)[0] ^ ((ODULong*) key)[1] );
{
	return Random( ((ODULong*) key)[0] ^ ((ODULong*) key)[1] );
}

ODULong OpenHashTable::HashTwoLongs(const void* key)
	// A standard hash function for an 8-byte key.
	// returns ( ((ODULong*) key)[0] ^ ((ODULong*) key)[1] );
{
	return ( ((ODULong*) key)[0] ^ ((ODULong*) key)[1] );
}

enum {
    OpenHashTable_kTop4Mask = 0xF0000000L,
    OpenHashTable_kRightShift = 28
};

ODULong OpenHashTable::HashString(register const ODUByte* key, register ODULong keyLength) 
	// Derived from P.J.Weinberger (from the "dragon" book). The left shifts
	// generate values distributed widely across the range of hash values even
	// for short strings, providing good "randomness". The important
	// principle is that by XORing high bits back into the low bits after
	// shifting, we keep the hash dependent on all the bits that have ever
	// been seen so far.
{
	register ODULong hash = 0;
    register ODULong top;

	++keyLength;
    while (--keyLength)
    {
		hash <<= 4;
		hash += (unsigned char) *key++;
		top = (hash & OpenHashTable_kTop4Mask);
		if (top)
		{
			hash ^= (top >> OpenHashTable_kRightShift);
			hash ^= top;
		}
    }
    return hash;
}


OpenHashTable::OpenHashTable(OpenHashKeyEqual equal, OpenHashKeyHash hash,
			  ODMemoryHeapID heap)
  : fTable(0L), fEntries(0L), fKeys(0L), fValues(0L), fFreeList(0L), 
    fEqual(equal), fHash(hash), fHeap(heap),
	fKeySize(0), fValSize(0),
	fSlots(0), fCount(0), fSeed(0), fThrowErrors(kODTrue)
	
		// Compare keys with equal, and hash them with hash.
		// All memory is allocated from the specified heap.
{
}
		
OpenHashTable::OpenHashTable(const OpenHashTable& otherTable)
  : fTable(0L), fEntries(0L), fKeys(0L), fValues(0L), fFreeList(0L), 
    fEqual(otherTable.fEqual), fHash(otherTable.fHash), fHeap(otherTable.fHeap),
	fKeySize(0), fValSize(0),
	fSlots(0), fCount(0), fSeed(0), fThrowErrors(kODTrue)
	// Use the same equal, hash, and heap as otherTable.
	// (Does *not* copy otherTable; use InitializeFromTable() and
	// CopyTable() to actually copy the contents of otherTable.)
{
}

void OpenHashTable::SetHeap(ODMemoryHeapID heap)
	// All memory is allocated from the specified heap.
	// This method does nothing after Initialize() has been called.
	// Generally this function is only called by hash tables delegating
	// to this one, which specify the heap in Initialize() functions
	// instead of in the constructor.
{
	if ( !fTable )
		fHeap = heap;
}

ODBoolean OpenHashTable::Initialize(ODULong sizeHint, 
	                     OpenHashSize keySize,
	                     OpenHashSize valueSize, 
						 ODBoolean throwErrors)
	// Initialize the table with sizeHint slots.  Keys will occupy keySize
	// bytes of space, and this amount of space will copied in/out of
	// functions.  Values will occupy valueSize bytes of space, and this
	// amount of space will copied in/out of functions.   valueSize can
	// be zero, causing values to be ignored.  The value of throwErrors
	// controls whether exceptions will be thrown when errors occur.
	// Note that if either keys or values have alignment requirements,
	// then keySize and valueSize must be such that keyPtr + keySize and
	// valuePtr + valueSize point to appropriately aligned structures.
	// Returns false only if memory could not be allocated. (If
	// throwErrors, throws kODErrOutOfMemory instead of returning false.) 
	// You can alternatively use InitAndCopyFrom() to copy another table.
{
	fKeySize = keySize;
	fValSize = valueSize;
	
	fThrowErrors = throwErrors;
	
	Clear(); // just in case this is not the first time.
	++fSeed;
	
	if (sizeHint < 3)
		sizeHint = 3;
	else if (sizeHint > 0x7FFFF) // avoid huge initial table size
		sizeHint = 0x7FFFF;
	fSlots = sizeHint;
	
	OpenHashVectors old;
	if ( NewVectors(&old, fSlots) )
		return kODTrue;
	else if (throwErrors)
	{
		THROW(kODErrOutOfMemory);
		return kODFalse; // we won't reach here
	}
	else
		return kODFalse;
}
	
ODBoolean OpenHashTable::InitAndCopyFrom(const OpenHashTable& otherTable)
	// Initialize this table so that it uses the same parameters
	// as otherTable; sizeHint is taken as otherTable.Count().
	// Then we call this->CopyTable(otherTable) to copy the contents.
	// Returns false only if memory could not be allocated. (If
	// throwErrors, throws kODErrOutOfMemory instead of returning false.) 
{
	if (Initialize(otherTable.Count(),
	                  otherTable.fKeySize,
	                  otherTable.fValSize,
	                  otherTable.fThrowErrors))
	{
		return this->CopyTable(otherTable);
	}
	else
		return kODFalse;
}

void OpenHashTable::RemoveEntry(void* keyPtr)
	// Remove the association with the specified key.  keyPtr must be
	// a POINTER to a key (e.g. if a key is struct Foo, then keyPtr must
	// be type struct Foo*). Nothing happens if there was no such
	// association.  This operation is fast and cheap because an old entry
	// is simply "unlinked" and set aside for reuse.  (Consider using 
	// ShrinkToFit() to reclaim space after numerous removals.)
	// (Note that if you need a copy of the old contents of the key or
	// value from a previous association, you should copy them first using
	// one of GetKey(), GetValue(), or GetKeyAndValue().)
{
	if (fTable)
	{
		OpenHashEntry** where = Find(keyPtr, (*fHash)(keyPtr));
		if (where)
		{
			OpenHashEntry* e = *where;
			*where = e->fLink; // unlink this entry
			PushFreeEntry(e);  // and put it in the free list
			++fSeed;
			--fCount;
		}
	}
}

ODBoolean OpenHashTable::Exists(void* keyPtr) const
	// Return whether there is an association with a key equal to (*keyPtr).
	// keyPtr must be a POINTER to a key (e.g. if a key is struct
	// Foo, then keyPtr must be type struct Foo*).
{
	if (fTable)
		return (Find(keyPtr, (*fHash)(keyPtr)) != 0L);
	return kODFalse;
}

void OpenHashTable::CopyTo(void* key, void* value, ODULong i) const
{
	if (key)
		memcpy(key, fKeys + (i * fKeySize), fKeySize);
	if (value && fValSize)
		memcpy(value, fValues + (i * fValSize), fValSize);
}

ODBoolean OpenHashTable::GetKeyAndValue(void* key, void* keyPtr, void* valuePtr) const
	// Roughly equivalent to GetKey(key, keyPtr), GetValue(key, valuePtr),
	// but faster.  Either keyPtr or valuePtr can be set to zero to
	// suppress copying that part of the association.
{
	if (fTable)
	{
		OpenHashEntry** where = Find(key, (*fHash)(key));
		if (where)
		{
			CopyTo(keyPtr, valuePtr, (*where) - fEntries);
			return kODTrue;
		}
	}
	return kODFalse;
}

ODBoolean OpenHashTable::GetKey(void* keyInPtr, void* keyOutPtr) const
	// Find the key in the table equal to (*keyInPtr) and return it in
	// (*keyOutPtr); returns false only if there was no association with
	// the given key.   keyInPtr and keyOutPtr must be POINTERs to keys
	// (e.g. if a key is struct Foo, then keyPtr must be type struct Foo*).
	// If the association is found, then <key-size> bytes are copied to the
	// address indicated by keyOutPtr; it is safe to use the same pointer
	// for keyInPtr and keyOutPtr because keyInPtr is not read again after
	// keyOutPtr is written.
{
	if (fTable)
	{
		OpenHashEntry** where = Find(keyInPtr, (*fHash)(keyInPtr));
		if (where)
		{
			ODULong i = ((*where) - fEntries); // index of key
			memcpy(keyOutPtr, fKeys + (i * fKeySize), fKeySize);
			return kODTrue;
		}
	}
	return kODFalse;
}

ODBoolean OpenHashTable::GetValue(void* keyPtr, void* valuePtr) const
	// Find and return the value associated with key in (*valuePtr);
	// returns false only if there was no association with the given key,
	// or if <value-size> is zero.  keyPtr must be a POINTER to a key, and
	// valuePtr must be a POINTER to a value (e.g. if a key is struct
	// Foo, then keyPtr must be type struct Foo*).
	// If the association is found, and if <value-size> is nonzero, then
	// <value-size> bytes are copied to the address indicated by valuePtr;
	// otherwise valuePtr is ignored.
{
	if (fTable && fValSize)
	{
		OpenHashEntry** where = Find(keyPtr, (*fHash)(keyPtr));
		if (where)
		{
			ODULong i = ((*where) - fEntries); // index of value
			memcpy(valuePtr, fValues + (i * fValSize), fValSize);
			return kODTrue;
		}
	}
	return kODFalse;
}

ODBoolean OpenHashTable::ReplaceEntry(void* keyPtr, void* valuePtr)
	// Replace and/or add an entry that associates key with value. keyPtr 
	// must be a POINTER to a key, and valuePtr must be a POINTER to a value
	// (e.g. if a key is struct Foo, then keyPtr must be type struct Foo*).
	// <key-size> bytes from keyPtr will be copied into the table, and 
	// <value-size> bytes from valuePtr will be be copied into the
	// table (if <value-size> is zero, valuePtr is ignored).
	// (Note that if you need a copy of the old contents of the key or
	// value from a previous association, you should copy them first using
	// one of GetKey(), GetValue(), or GetKeyAndValue().)
	// If the addition of this entry makes the table's entry count exceed
	// the current capacity of the table, then the table grows by some
	// percentage (say 50%), leaving some empty slots for future additions.
	// False is returned only if memory could not be allocated. (If
	// throwErrors, throws kODErrOutOfMemory instead of returning false.) 
{
	if (fTable)
	{
		ODULong hash = (*fHash)(keyPtr);
		OpenHashEntry** where = Find(keyPtr, hash);
		if ( !where ) // add a new entry
		{
			OpenHashEntry* e = PopFreeEntry();
			if ( !e )
			{
				if ( !Grow() )
				{
					if (fThrowErrors)
						THROW(kODErrOutOfMemory);
					return kODFalse;
				}
				e = PopFreeEntry();
			}
			++fCount;
			where = fTable + (hash % fSlots);
			e->fLink = *where;
			*where = e; // new entries are pushed on front of bucket
#ifdef OPENHASH_CACHE_HASH
			e->InitEntry(hash);
#endif
		}
		++fSeed; // indicate hash table is modified
		ODULong i = (*where) - fEntries;
		memcpy(fKeys + (i * fKeySize), keyPtr, fKeySize);
		if (fValSize)
			memcpy(fValues + (i * fValSize), valuePtr, fValSize);
	}
	return kODTrue;
}


ODBoolean OpenHashTable::TrueAction(void* k, void* v, ODULong s, void* r)
	// This function ignores its arguments and always returns true.
	// It is meant to be passed as an action argument to Intersect().
{
	k = v = r = &s; // ignore these
	return kODTrue;
}

ODBoolean OpenHashTable::Intersect(OpenHashTable* ht, OpenHashAction action, void* ref)
	// Walk this table and compare with table ht, calling action (with ref
	// as one argument) once for every association in this table which has
	// a key equal to a key in ht.  (Note: the value of slot passed to
	// action is meaningless.) If action ever returns true, Intersect()
	// returns immediately without examining any more associations. 
	// Intersect() returns true only if action returns true (thus, for
	// example, if either table is empty then action is never called and
	// Intersect() returns false). If the caller wishes to build a data
	// structure (e.g. a list) that represents the intersection, this can
	// be maintained in ref.  Or, if the caller wants a boolean yes or no
	// (is the intersection non-empty?) then action merely needs to always
	// return true (use TrueAction()).  Table ht must have the
	// same hash and equal functions as in this table. You cannot add or
	// remove associations in this table during the intersection, but you
	// can in ht. 
{
	OpenHashEntry** bucket = fTable;
	OpenHashEntry** end = fTable + fSlots;
	OpenHashEntry* e;
	OpenHashEntry** where;
	ODULong i; // index of key or value
	char* key;
	char* value = fValues; // nil when fValSize == 0

	while (bucket < end)
	{
		e = *bucket++;
		if ( e ) // did we find an old entry in this slot?
		{
			while ( e ) // iterate over all old entries in this old slot
			{
				i = e - fEntries;
				key = fKeys + (i * fKeySize);
				// look for a matching key in ht:
#ifdef OPENHASH_CACHE_HASH
				where = ht->Find(key, e->Hash());
#else
				where = ht->Find(key, (*fHash)(key));
#endif
				if (where)
				{
					if (value) 
						value = fValues + (i * fValSize);
					if ( (*action)(key, value, /*slot*/ 0, ref) )
						return kODTrue;
				}
				e = e->fLink;
			}
		}
	}
	return kODFalse;
}

static ODBoolean 
OpenHashTable_CopyAction(void* key, void* value, ODULong /*slot*/, void* ref)
	// This is the action function used by CopyTable() to copy entries
	// into one table (ref) from another table being walked.  Note that
	// we must invert the sense of the boolean return, because Walk() stops
	// when the first true is returned, but ReplaceEntry() returns *false*
	// on failure (and we only want to stop on failure).
{
	return ( ! ((OpenHashTable*) ref)->ReplaceEntry(key, value));
}
	
ODBoolean OpenHashTable::CopyTable(const OpenHashTable& otherTable)
	// Copy all of otherTable's entries into this table such that the
	// result is the union of the previous contents of this table and
	// otherTable.  If this table was empty, the result is a simple copy.
	// If this table contains entries that are also in otherTable, they
	// are replaced with otherTable's entries (beware losing pointers
	// and creating garbage this way). Othertable should have the same
	// equal and hash functions, as well as the same key and value sizes.  
	// Returns false only if memory could not be allocated. (If
	// throwErrors, throws kODErrOutOfMemory instead of returning false.)
	// The implementation is just an optimized way of iterating over
	// entries in otherTable and calling this->ReplaceEntry() for each. 
{
	// This would be very similar to Walk(), so let's just call Walk()
	// to reduce the size of the code generated.  Note that we must
	// invert the sense of the Walk() return value.
	if (this != &otherTable)
		return ( ! otherTable.Walk(OpenHashTable_CopyAction, (void*) this));
	else
		return kODTrue;
}

ODBoolean OpenHashTable::Walk(OpenHashAction action, void* ref) const
	// Call action once for every association in the hash table, passing 
	// through ref each time, as well as the slot in the table containing
	// the association.  If action ever returns true, the walk is terminated
	// immediately, rather than visiting every association. Walk() only
	// returns true if action returns true (thus, an empty table never even
	// calls action and returns false). This function is useful for printing
	// the distribution of keys in a table to evaluate the quality of hashing.
	// (It's not appropriate to expose slots in the table with an iterator.)
	// You cannot add or remove associations during the walk.
{
	OpenHashEntry** bucket = fTable;
	OpenHashEntry** end = fTable + fSlots;
	OpenHashEntry* e;
	ODULong i; // index of key or value
	char* key;
	char* value = fValues; // nil when fValSize == 0

	while (bucket < end)
	{
		ODULong slot = bucket - fTable;
		e = *bucket++;
		if ( e ) // did we find an old entry in this slot?
		{
			while ( e ) // iterate over all old entries in this old slot
			{
				i = e - fEntries;
				key = fKeys + (i * fKeySize);
				if (value) 
					value = fValues + (i * fValSize);
				if ( (*action)(key, value, slot, ref) )
					return kODTrue;
				e = e->fLink;
			}
		}
	}
	return kODFalse;
}

ODBoolean OpenHashTable::ShrinkToFit(OpenHashSize extraSlots)
	// Change the capacity of the table to Count() + extraSlots.  This is
	// the principal means of recovering storage after numerous removals.
	// Presumably an application watches the relationship between Count()
	// and Capacity(), and shrinks the table when count falls below some
	// threshold. Returns false only if memory could not be allocated. (If
	// throwErrors, throws kODErrOutOfMemory instead of returning false.)
{
	// This function is very similar to Grow(), except that in general there
	// are unused entries in the old table, and we have to actually iterate over
	// the old table to figure out what to transfer to the new.  So there is not
	// a lot of code in common after the initialization of necessary locals.

	if (fTable)
	{
		OpenHashSize nominalSlots = fCount + extraSlots;
		OpenHashSize newSlots = (nominalSlots > 3)? nominalSlots : 3;
		
		OpenHashVectors old;
		if ( !NewVectors(&old, newSlots) )
		{
			if (fThrowErrors)
				THROW(kODErrOutOfMemory);
			return kODFalse;
		}		
		// iterate over all old entries, copying them to new entries
		// and hashing the new entries into their correct buckets:
	
		OpenHashEntry** oldBucket = old.fTable;
		OpenHashEntry** oldEnd = old.fTable + old.fSlots;
		OpenHashEntry* e;
		OpenHashEntry* olde;
		ODULong n; // new index
		ODULong o; // old index
	
		while (oldBucket < oldEnd)
		{
			olde = *oldBucket++;
			if ( olde ) // did we find an old entry in this slot?
			{
				while (olde) // iterate over all old entries in this old slot
				{
					e = PopFreeEntry();   // pop a new entry
					n = e - fEntries;
					o = olde - old.fEntries;
					
					char* key = fKeys + (n * fKeySize);
					memcpy(key, old.fKeys + (o * fKeySize), fKeySize);
					if (fValSize)
					{
						char* value = fValues + (n * fValSize);
						memcpy(value, old.fValues + (o * fValSize), fValSize);
					}
					
#ifdef OPENHASH_CACHE_HASH
					e->fHash = olde->fHash;
					OpenHashEntry** where = fTable + (e->Hash() % fSlots);
#else
					OpenHashEntry** where = fTable + ((*fHash)(key) % fSlots);
#endif
					e->fLink = *where; // insert new entry into its bucket
					*where = e;
					
					olde = olde->fLink;
				}
			}
		}
		old.Destroy();   // no longer need old vectors
	}
	return kODTrue;
}

ODBoolean OpenHashTable::Grow()
	// returns false only if out of memory.
	// Increase the table size by 50%.  The old table is assumed to be exactly
	// full, because the free list is empty, which means that every entry is in
	// use.  This means we can efficiently move the old entries to the new entry
	// vector by block-moving them wholesale.  The structure of the old table
	// can be ignored.  Then the new entries need to be hashed into the new
	// table.  The new free list begins immediately after the last copied entry.
{
	OpenHashSize newSlots = ((fSlots * 3) / 2) + 1; // 50% growth

	OpenHashVectors old;
	if ( !NewVectors(&old, newSlots) )
		return kODFalse;

	// transfer all the old entries to the new table:
	memcpy(fEntries, old.fEntries, old.fSlots * sizeof(OpenHashEntry));
	memcpy(fKeys, old.fKeys, old.fSlots * fKeySize);
	if (fValSize)
		memcpy(fValues, old.fValues, old.fSlots * fValSize);

	// the new free list starts after the last copied entry:
	fFreeList = fEntries + old.fSlots;
	
	// hash all the copied entries into their correct table buckets:
	OpenHashEntry* newEntries = fEntries - 1;
	char* key = fKeys;
	
	while (++newEntries < fFreeList)
	{
#ifdef OPENHASH_CACHE_HASH
		OpenHashEntry** where = fTable + (newEntries->Hash() % fSlots);
#else
		OpenHashEntry** where = fTable + ((*fHash)(key) % fSlots);
		key += fKeySize;
#endif
		newEntries->fLink = *where;
		*where = newEntries;
	}
	old.Destroy();   // no longer need old vectors
	
	return kODTrue;
}

//==============================================================================
// OpenHashTableIterator
//==============================================================================

OpenHashTableIterator::OpenHashTableIterator(OpenHashTable* table)
	: fTable(table), fSeed(table->Seed()), 
	  fBucket(0), fRef(0), fCurrent(0), fAfter(0)
{
	// note that fCurrent == 0 indicates the iteration is exhausted.
}

OpenHashTableIterator::~OpenHashTableIterator()
{
	fSeed = 0;
	fTable = 0;
}


ODBoolean OpenHashTableIterator::Current(void* keyPtr, void* valuePtr)
    // Copies the same <key, value> pair as the last call to either
    // First() or Next().  Either keyPtr or valuePtr can be set to 
	// zero to suppress copying that part of the assocation.  valuePtr
	// will also be ignored if <value-size> is zero.  keyPtr must 
	// be a POINTER to a key, and valuePtr must be a POINTER to a value
	// (e.g. if a key is struct Foo, then keyPtr must be type struct Foo*).
	// keyPtr or valuePtr are ignored if there are no more associations
	// (use IsNotComplete() to check).  If the table is out of sync with
	// the iterator (has been modified since First() other than by
	// RemoveCurrent()), then exception kODErrIteratorOutOfSync is thrown.
{
	if (fSeed != fTable->Seed())
		THROW(kODErrIteratorOutOfSync);
		
	if (fCurrent)
	{
		fTable->CopyTo(keyPtr, valuePtr, fCurrent - fTable->fEntries);
		return kODTrue;
	}
	else
		return kODFalse;
}

void OpenHashTableIterator::RemoveCurrent()
    // Efficiently removes the current <key, value> pair from the table.
	// This method works the same whether you are iterating with
	// associations or with pointers.  If the table is out of sync with
	// the iterator (has been modified since First() other than by
	// RemoveCurrent()), then exception kODErrIteratorOutOfSync is thrown.
{
	// This function has been written so that it can be called more than 
	// once for the same current assoc without doing any harm.  Also,
	// Current() can be called *after* removing the current assoc
	// without doing any harm (the iterator still references the current
	// assoc after we unlink it from the table).
	
	if (fSeed != fTable->Seed())
		THROW(kODErrIteratorOutOfSync);
		
	if (fCurrent && (*fRef != fAfter))
	{
		fTable->PushFreeEntry(fCurrent); // put it in free list
		*fRef = fAfter;                  // unlink current entry
		
		// if fAfter != 0, then fRef is still an appropriate reference for
		// the next entry in the iteration, so we will not need to update
		// fRef in NextAssoc().

		--fTable->fCount;                // table has one fewer entries
		fSeed = ++fTable->fSeed;         // changed, but *we* are in sync
	}
}

void OpenHashTableIterator::First(void* keyPtr, void* valuePtr)
	// Resets the iteration and copies the "first" <key, value> pair 
	// in the hash table.  Either keyPtr or valuePtr can be set to 
	// zero to suppress copying that part of the assocation.  valuePtr
	// will also be ignored if <value-size> is zero.  keyPtr must 
	// be a POINTER to a key, and valuePtr must be a POINTER to a value
	// (e.g. if a key is struct Foo, then keyPtr must be type struct Foo*).
	// keyPtr or valuePtr are ignored if there are no more associations
	// (use IsNotComplete() to check).  The iterator and the table are
	// always in sync after First().
{
	// Note that we don't care if the seed is out of sync, because this
	// operation puts us in sync.
	
	fSeed = fTable->Seed();
	
	OpenHashEntry** bucket = fTable->fTable;
	OpenHashEntry** end = bucket + fTable->fSlots; // one past last slot
	OpenHashEntry* e;
	fCurrent = 0L;
	
	--bucket;
	while (++bucket < end)
	{
		e = *bucket;
		if ( e ) // did we find the first entry?
		{
			fBucket = fRef = bucket;
			fCurrent = e;
			fAfter = e->fLink;
			// We need to maintain fAfter independently, because removal
			// of the current assoc overwrites fCurrent->fLink.

			fTable->CopyTo(keyPtr, valuePtr, e - fTable->fEntries);
			break; // stop after finding the first entry
		}
	}
}

void OpenHashTableIterator::Next(void* keyPtr, void* valuePtr)
	// Continues the iteration and copies the "next" <key, value> 
	// pair in the hash table.  Either keyPtr or valuePtr can be set to 
	// zero to suppress copying that part of the assocation.  valuePtr
	// will also be ignored if <value-size> is zero.  keyPtr must 
	// be a POINTER to a key, and valuePtr must be a POINTER to a value
	// (e.g. if a key is struct Foo, then keyPtr must be type struct Foo*).
	// keyPtr or valuePtr are ignored if there are no more associations
	// (use IsNotComplete() to check).  If the table is out of sync with
	// the iterator (has been modified since First() other than by
	// RemoveCurrent()), then exception kODErrIteratorOutOfSync is thrown.
{
	if (fSeed != fTable->Seed())
		THROW(kODErrIteratorOutOfSync);
		
	if (fCurrent)
	{
		if (fAfter) // more in the same bucket
		{
			if (*fRef != fAfter) // fCurrent not removed? must update fRef?
				fRef = &fCurrent->fLink;        
			fCurrent = fAfter;
			fAfter = fAfter->fLink;
		}
		else // find the next non-empty bucket
		{
			OpenHashEntry** end = fTable->fTable + fTable->fSlots;
			OpenHashEntry* e;
			fCurrent = 0L;
			while (++fBucket < end)
			{
				e = *fBucket;
				if ( e ) // did we find an entry?
				{
					fRef = fBucket;
					fCurrent = e;
					fAfter = e->fLink;
					break;
				}
			}
		}
		if (fCurrent)
		{
			fTable->CopyTo(keyPtr, valuePtr, fCurrent - fTable->fEntries);
		}
	}
}
