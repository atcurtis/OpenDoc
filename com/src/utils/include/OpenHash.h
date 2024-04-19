//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   OpenHashTable
//		OpenHashTableIterator
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
// @(#) 1.3 com/src/utils/include/OpenHash.h, odutils, od96os2, odos29646d 7/15/96 18:01:24 [ 11/15/96 15:29:01 ]
/*
	File:		OpenHash.h

	Contains:	OpenHashTable, a hash table with "open" hashing

	Owned by:	David McCusker

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	
*/

#ifndef _OPENHASH_
#define _OPENHASH_ 1

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _ODMEMORY_
#include "ODMemory.h"
#endif

//==============================================================================
// Constants
//==============================================================================

//==============================================================================
// Scalar Types
//==============================================================================

typedef ODBoolean (*OpenHashKeyEqual)(const void* key, const void* testKey);
typedef ODULong (*OpenHashKeyHash)(const void* key);

typedef unsigned int OpenHashSize; // must be unsigned, (similar to size_t)

typedef ODBoolean (*OpenHashAction)(void* key, void* value, ODULong slot, void* ref);
	/* OpenHashAction supports walking hash tables.  Returning true causes
	 * a walk to terminate early (false continues the walk).  This allows
	 * a walk to be invoked just to see if it finds something interesting.
	 */

//==============================================================================
// Classes defined in this interface
//==============================================================================

class OpenHashTable;
class OpenHashTableIterator;

//==============================================================================
// Classes used by this interface
//==============================================================================

class OpenHashEntry; // private to the implementation
class OpenHashVectors; // private to the implementation


//==============================================================================
// OpenHashTable: theory of operation
//==============================================================================

/*
	OpenHashTable is a hash table that implements "open" hashing (such that
	collisions are handled by chaining together the members of a bucket). This
	permits efficient removal of hash table associations (as opposed to closed
	hashing, which might require a non-trivial amount of entry movement after
	a removal).  In particular, this implementation of open hashing supports
	removal during iterations.
	
	A table is logically a set of <key, value> pairs, where no key is equal to
	another. This defines a mapping that associates keys with values.
	OpenHashTable manages the space for both keys and values; keys and values
	are copied into and out of the hash table through the interface.  The
	amount of space required for each key and value is specified when the hash
	table is initialized.  Values may be specified as zero-sized, causing the
	the table to allocate space for keys only.  Users might want to do this
	when a key contains all the information of interest, and a value would
	be redundant. (Note that keys may *not* be zero-sized.)  If users want
	to manage the memory for their own keys and values, it is simple to use
	pointers for the keys and values, and specify the size as sizeof(void*).
	
	The only requirements for keys are that they have appropriate
	OpenHashKeyEqual and OpenHashKeyHash functions, which are related to 
	each other by the following invariant:
	    (*equal)(key1, key2) implies (*hash)(key1) == (*hash)(key2)
	
	Assuming a full table, the storage overhead for this implementation is
	two longwords per <key, value> association.  (If OPENHASH_CACHE_HASH is
	defined, another longword of overhead is added to cache the hash of keys,
	and the speed of lookup operations and changes in table size will be
	significantly increased.)
	
	This implementation uses mod (%) to calculate buckets from hashes.  If
	masking (&) had been used, the table would have been constrained to sizes
	that are powers of two.  Masking might be slightly faster, but causes
	poor distribution if the low bits of hashes are not very pseudo-random.
*/


class OpenHashTable {	
public:
		
	//=============================
	// Hashing
	
	static ODULong StdHash(const void* key);
		// A standard hash function useable when keys are pointers or ints.
		// Hash is defined as the pointer cast to an integer.  Note that since
		// a pointer to the key is passed, the argument type is actually
		// pointer to a pointer.
		// returns *(ODULong*) key;

	static ODULong Random(ODULong seed);
		// Pseudo-random number generator with period 2**31-1 (i.e. all
		// values in 1..2**31-1 are visted before repetition). Useful
		// for hashing one or more longwords. Derived from published
		// literature on good random number generators (the names
		// Park and Miller come to mind).
	
	static ODULong HashString(const ODUByte* key, ODULong keyLength);
		// This is a generic hashing utility that might sometimes be used to
		// implement a hash function passed to the OpenHashTable constructor;
		// the keyLength arg must be curried because only key is passed
		// to a hash function.  HashString() is conceptually derived
		// from P.J.Weinberger (from the "dragon" book).
	
	static ODULong RandomOneLong(const void* key);
		// A standard hash function for a 4-byte key, based on Random().
		// returns Random( *(ODULong*) key );
	
	static ODULong RandomTwoLongs(const void* key);
		// A standard hash function for an 8-byte key, based on Random().
		// returns Random( ((ODULong*) key)[0] ^ ((ODULong*) key)[1] );
	
	static ODULong HashTwoLongs(const void* key);
		// A standard hash function for an 8-byte key, where the longwords
		// in the keys are likely already largish values (like pointers).
		// returns ( ((ODULong*) key)[0] ^ ((ODULong*) key)[1] );
		
	//=============================
	// Equality

	static ODBoolean StdEqual(const void* key, const void* testKey);
		// A standard equal function useable when keys are pointers or ints.
		// Equality is defined as pointer equality.  Note that since
		// a pointer to the key is passed, the argument type is actually
		// pointer to a pointer.
		// returns ( *(void**) key == *(void**) testKey);
		
	static ODBoolean EqualTwoLongs(const void* key, const void* testKey);
		// A standard equal function for an 8-byte key. Compares the
		// first two longwords of each keys as unsigned longs.

	//=============================
	// Making and Breaking
		
	OpenHashTable(OpenHashKeyEqual equal = StdEqual, 
	              OpenHashKeyHash hash = StdHash,
	              ODMemoryHeapID heap=kDefaultHeapID);
		// Compare keys with equal, and hash them with hash.
		// All memory is allocated from the specified heap.
			
	OpenHashTable(const OpenHashTable& otherTable); 
		// Use the same equal, hash, and heap as otherTable.
		// (Does *not* copy otherTable; use InitializeAndCopy() 
		// to actually copy the contents of otherTable.)

	void SetHeap(ODMemoryHeapID heap=kDefaultHeapID);
		// All memory is allocated from the specified heap.
		// This method does nothing after Initialize() has been called.
		// Generally this function is only called by hash tables delegating
		// to this one, which specify the heap in Initialize() functions
		// instead of in the constructor.
	
	ODBoolean Initialize(ODULong sizeHint=5, 
	                     OpenHashSize keySize=sizeof(void*),
	                     OpenHashSize valueSize=sizeof(void*), 
						 ODBoolean throwErrors=kODTrue);
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
		
	ODBoolean InitAndCopyFrom(const OpenHashTable& otherTable);
		// Initialize this table so that it uses the same parameters
		// as otherTable; sizeHint is taken as otherTable.Count().
		// Then we call this->CopyTable(otherTable) to copy the contents.
		// Returns false only if memory could not be allocated. (If
		// throwErrors, throws kODErrOutOfMemory instead of returning false.) 
		
	ODBoolean CopyTable(const OpenHashTable& otherTable);
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

	~OpenHashTable();
		
	//=============================
	// Association operations

	ODBoolean        ReplaceEntry(void* keyPtr, void* valuePtr);
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

	void             RemoveEntry(void* keyPtr);
		// Remove the association with the specified key.  keyPtr must be
		// a POINTER to a key (e.g. if a key is struct Foo, then keyPtr must
		// be type struct Foo*). Nothing happens if there was no such
		// association.  This operation is fast and cheap because an old entry
		// is simply "unlinked" and set aside for reuse.  (Consider using 
		// ShrinkToFit() to reclaim space after numerous removals.)
		// (Note that if you need a copy of the old contents of the key or
		// value from a previous association, you should copy them first using
		// one of GetKey(), GetValue(), or GetKeyAndValue().)

	ODBoolean        GetValue(void* keyPtr, void* valuePtr) const;
		// Find and return the value associated with key in (*valuePtr);
		// returns false only if there was no association with the given key,
		// or if <value-size> is zero.  keyPtr must be a POINTER to a key, and
		// valuePtr must be a POINTER to a value (e.g. if a key is struct
		// Foo, then keyPtr must be type struct Foo*).
		// If the association is found, and if <value-size> is nonzero, then
		// <value-size> bytes are copied to the address indicated by valuePtr;
		// otherwise valuePtr is ignored.

	ODBoolean        GetKey(void* keyInPtr, void* keyOutPtr) const;
		// Find the key in the table equal to (*keyInPtr) and return it in
		// (*keyOutPtr); returns false only if there was no association with
		// the given key.   keyInPtr and keyOutPtr must be POINTERs to keys
		// (e.g. if a key is struct Foo, then keyPtr must be type struct Foo*).
		// If the association is found, then <key-size> bytes are copied to the
		// address indicated by keyOutPtr; it is safe to use the same pointer
		// for keyInPtr and keyOutPtr because keyInPtr is not read again after
		// keyOutPtr is written.

	ODBoolean        GetKeyAndValue(void* key, void* keyPtr, void* valuePtr) const;
		// Roughly equivalent to GetKey(key, keyPtr), GetValue(key, valuePtr),
		// but faster.  Either keyPtr or valuePtr can be set to zero to
		// suppress copying that part of the association.
	
	ODBoolean        Exists(void* keyPtr) const;
		// Return whether there is an association with a key equal to (*keyPtr).
		// keyPtr must be a POINTER to a key (e.g. if a key is struct
		// Foo, then keyPtr must be type struct Foo*).
		
	//=============================
	// Table parameters

	ODULong          Count() const; // number of associations
	ODULong          Capacity() const; // current number of slots
	ODULong          Seed() const; // changes whenever table is modified

	OpenHashSize     KeySize() const; // size of keys: <key-size>
	OpenHashSize     ValueSize() const; // size of values: <value-size>

	ODBoolean        ThrowErrors() const;
	void             SetThrowErrors(ODBoolean b);
	
	ODBoolean        ShrinkToFit(OpenHashSize extraSlots=3);
		// Change the capacity of the table to Count() + extraSlots.  This is
		// the principal means of recovering storage after numerous removals.
		// Presumably an application watches the relationship between Count()
		// and Capacity(), and shrinks the table when count falls below some
		// threshold. Returns false only if memory could not be allocated. (If
		// throwErrors, throws kODErrOutOfMemory instead of returning false.)
	
	//=============================
	// comparing hash tables:
	ODBoolean        Intersect(OpenHashTable* ht, OpenHashAction action, void* ref);
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

	static ODBoolean TrueAction(void* k, void* v, ODULong s, void* r);
		// This function ignores its arguments and always returns true.
		// It is meant to be passed as an action argument to Intersect().

	//=============================
	// testing & debugging:
	ODBoolean        Walk(OpenHashAction action, void* ref) const;
		// Call action once for every association in the hash table, passing 
		// through ref each time, as well as the slot in the table containing
		// the association.  If action ever returns true, the walk is terminated
		// immediately, rather than visiting every association. Walk() only
		// returns true if action returns true (thus, an empty table never calls
		// action and returns false). This function is useful for printing the
		// distribution of keys in a table to evaluate the quality of hashing.
		// (It's not appropriate to expose slots in the table with an iterator.)
		// You cannot add or remove associations during the walk.
	
protected:
	OpenHashEntry**  fTable;       // hash buckets (numbering fSlots)
	OpenHashEntry*   fEntries;     // block of fSlots entries
	char*            fKeys;        // block of fSlots keys
	char*            fValues;      // block of fSlots values
	OpenHashEntry*   fFreeList;    // free list of entries
	OpenHashKeyEqual fEqual;       // used to compare keys
	OpenHashKeyHash  fHash;        // used to hash keys
	ODMemoryHeapID   fHeap;        // used to allocate memory
	OpenHashSize     fSlots;       // current size (capacity) of table
	ODULong          fCount;       // number of entries in table
	ODULong          fSeed;        // used to validate iterators
	OpenHashSize     fKeySize;     // size of keys
	OpenHashSize     fValSize;     // size of values
	ODBoolean        fThrowErrors; // throw exception on errors

	OpenHashEntry**  Find(void* key, ODULong hash) const;
	ODBoolean        Grow();
	
	void             Clear();
	OpenHashEntry**  NewTable(OpenHashSize slots) const;
	OpenHashEntry*   NewEntries(OpenHashSize slots) const;
	char*            NewKeys(OpenHashSize slots) const;
	char*            NewValues(OpenHashSize slots) const;

	ODBoolean        NewVectors(OpenHashVectors* old, OpenHashSize slots);
	
	OpenHashEntry*   PopFreeEntry();
	void             PushFreeEntry(OpenHashEntry* e);
	
	void             CopyTo(void* key, void* value, ODULong i) const;
private:			  	
	friend class OpenHashTableIterator;
	friend class OpenHashVectors;
};

inline ODULong OpenHashTable::Capacity() const
{
	return fSlots;
}

inline ODULong OpenHashTable::Count() const
{
	return fCount;
}

inline ODULong OpenHashTable::Seed() const
{
	return fSeed;
}

inline OpenHashSize OpenHashTable::KeySize() const
{
	return fKeySize;
}

inline OpenHashSize OpenHashTable::ValueSize() const
{
	return fValSize;
}

inline ODBoolean OpenHashTable::ThrowErrors() const
{
	return fThrowErrors;
}

inline void OpenHashTable::SetThrowErrors(ODBoolean b)
{
	fThrowErrors = b;
}


//==============================================================================
// OpenHashTableIterator
//
//	This iterator is only meant to be used in the the context of a for loop,
//	e.g.:
//
//  struct MyKey key;
//  struct MyValue value;
//	OpenHashTableIterator i(table);
//	for (i.First(&key, &value); i.IsNotComplete(); i.Next(&key, &value))
//	{
//		...
//	}
//
//==============================================================================

class OpenHashTableIterator
{
  public:
	OpenHashTableIterator(OpenHashTable* table);
	~OpenHashTableIterator();	

	void              First(void* keyPtr, void* valuePtr);
		// Resets the iteration and copies the "first" <key, value> pair 
		// in the hash table.  Either keyPtr or valuePtr can be set to 
		// zero to suppress copying that part of the assocation.  valuePtr
		// will also be ignored if <value-size> is zero.  keyPtr must 
		// be a POINTER to a key, and valuePtr must be a POINTER to a value
		// (e.g. if a key is struct Foo, then keyPtr must be type struct Foo*).
		// keyPtr or valuePtr are ignored if there are no more associations
		// (use IsNotComplete() to check).  The iterator and the table are
		// always in sync after First().
	
	void              Next(void* keyPtr, void* valuePtr);
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
	
	ODBoolean         Current(void* keyPtr, void* valuePtr);
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

	void              RemoveCurrent();
	    // Efficiently removes the current <key, value> pair from the table.
		// This method works the same whether you are iterating with
		// associations or with pointers.  If the table is out of sync with
		// the iterator (has been modified since First() other than by
		// RemoveCurrent()), then exception kODErrIteratorOutOfSync is thrown.

	ODBoolean		  IsNotComplete() const;
		// Returns true if the last call to First(), Next() or Current()
		// accessed an association in the table.

	ODBoolean		  IsOutOfSync() const;
		// Returns whether the iterator is out of sync with the table.
		// Users who do not want an kODErrIteratorOutOfSync exception to be
		// thrown from Next(), Current(), or RemoveCurrent() should avoid
		// calling those functions when IsOutOfSync() returns true.
		// (The iterator becomes out of sync when the table is modified other
		// than by RemoveCurrent() since the last call to First().)

  private:
	OpenHashTable*   fTable;    // the table we are iterating over
	ODULong          fSeed;     // should match table's seed
	OpenHashEntry**  fBucket;   // current bucket in the table
	OpenHashEntry**  fRef;      // normally (*fRef == fCurrent), for removal
	OpenHashEntry*   fCurrent;  // the current entry
	OpenHashEntry*   fAfter;    // normally fCurrent->fLink
};

inline ODBoolean OpenHashTableIterator::IsNotComplete() const
{
	return (fCurrent != 0);
}

inline ODBoolean OpenHashTableIterator::IsOutOfSync() const
{
	return (fSeed != fTable->Seed());
}


#endif
// _OPENHASH_
