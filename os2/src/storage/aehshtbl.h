/* @(#)Z 1.3 os2/src/storage/aehshtbl.h, oddataxfer, od96os2, odos29646d 96/11/15 15:50:01 (96/08/23 01:42:58) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: oddataxfer
 *
 *   CLASSES:   AEHashTable
 *		AEHashTableIterator
 *
 *   ORIGINS: 82,27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1995,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *   	
 *   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 *   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 *   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 *   OR PERFORMANCE OF THIS SOFTWARE.
 */
/*====END_GENERATED_PROLOG========================================
 */

/********************************************************************/
/*                                                                  */
/* Copyright (C) Apple Computer, Inc., 1994                         */
/*                                                                  */
/********************************************************************/

/*
  File:    AEHshTbl.h

  Contains:  Interface to AppleEvent hash table code and XMPAEHashTable
        class.

  Owned by:  Nick Pilch

  Copyright:  © 1993-94 by Apple Computer, Inc., all rights reserved.

*/

#ifndef _AEHSHTBL_
#define _AEHSHTBL_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifdef _PLATFORM_OS2_
#include <odtypesp.h>
#endif

//==============================================================================
// Theory of Operation
//==============================================================================

/*
  C Interface to some of the routines in HashTabl.A and class wrapper.

  The class simply provides an object-oriented wrapper to the AE hash table
  functions found in HashTable.A. It removes access to the MemHooks parameter. (MemHooks is set to NULL and SysHeap is set to false.)
  It hides the Table parameter since that is a member variable of the class.
*/

//==============================================================================
// Scalar Types
//==============================================================================

typedef void* ODEntryPtr;
typedef void* ODKeyPtr;

#ifndef _PLATFORM_OS2_
typedef  Handle HashTable;
#else
class    HCollection;
typedef  HCollection *HashTable;
class    HCollectionCursor;
typedef  HCollectionCursor *HashTableCursor;
#endif // ! _PLATFORM_OS2_

//==============================================================================
// Error Codes
//==============================================================================

const ODSShort kAEErrAlreadyExists  = -1722;
const ODSShort kAEErrNotFound    = -1723;
const ODSShort kAEErrEndOfTable  = -1724;


//==============================================================================
// Classes defined in this interface
//==============================================================================

#ifdef _PLATFORM_OS2_
class    HCollection;
typedef  HCollection *HashTable;
class    HCollectionCursor;
typedef  HCollectionCursor *HashTableCursor;
#endif

class AEHashTable;
class AEHashTableIterator;

//==============================================================================
// AEHashTable
//==============================================================================

class AEHashTable
{  
#ifndef _PLATFORM_OS2_
  friend class AEHashTableIterator;
#endif

  public:

  AEHashTable();
  ODVMethod void Initialize(ODULong numEntries, ODUShort keySize,
                ODUShort valueSize, ODBoolean inSysHeap);
    // kODErrOutOfMemory is thrown if the table cannot be created.

  virtual ~AEHashTable();

  ODVMethod void      ReplaceEntry(ODKeyPtr key, ODEntryPtr value);
    // Replace and/or add an entry. Pass a POINTER to the key as well as a
    //  a POINTER to the value to be added. kODErrOutOfMemory is thrown if
    //  the entry cannot be added.

  ODVMethod void      RemoveEntry(ODKeyPtr key);
    // Pass a POINTER to the key.

  ODVMethod ODBoolean  GetValue(ODKeyPtr key, ODEntryPtr value);
    // Pass a POINTER to the key as well as a POINTER to the value to be
    //  retrieved. kODTrue is returned if the key was found, kODFalse
    //  otherwise.
    
  ODVMethod ODBoolean  Exists(ODKeyPtr key);
    // Check to see if a key exists. This function is no faster than
    //  GetValue and should probably only be used when you do not intend to
    //  fetch the value immediately. kODTrue is returned if the key exists,
    //  kODFalse, otherwise.
  
#ifndef _PLATFORM_OS2_
  protected:
#else
  private:
#endif

  HashTable  fAEHashTable;
  ODUShort   fValueSize;
  
#ifdef _PLATFORM_OS2_
  int        fKeySize, fEntrySize;
  friend class AEHashTableIterator;
#endif

  HashTable  GetAEHashTable();
};

//==============================================================================
// AEHashTableIterator
//
//  This iterator is only meant to be used in the the context of a for loop,
//  e.g.:
//
//  AEHashTableIterator iter;
//  for (iter.First(key, value); iter.IsNotComplete(); iter.Next(key, value))
//  {
//    ...
//  }
//
//==============================================================================

class AEHashTableIterator
{
  public:
  AEHashTableIterator(AEHashTable* table);
  ~AEHashTableIterator();
  
  void      First(ODKeyPtr key, ODEntryPtr value);
  void      Next(ODKeyPtr key, ODEntryPtr value);
  ODBoolean    IsNotComplete();
  private:
#ifndef _PLATFORM_OS2_
  AEHashTable* fTable;
  ODSLong    fIndex;
  ODBoolean    fDone;
  ODBoolean    GetNext(ODKeyPtr key, ODEntryPtr value);
#else
  HashTable       fTable;
  HashTableCursor cursor;
#endif // ! _PLATFORM_OS2_
};

#endif // _AEHSHTBL_
