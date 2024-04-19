/* @(#)Z 1.6 com/src/cm/TOCEnts.h, odstorage, od96os2, odos29646d 96/11/15 15:27:16 (96/10/29 09:19:33) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES: none
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

/*
	File:		 TOCEnts.h  

	Contains:	Container Manager TOC Entry Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <3>	 5/10/94	EL		#1162327. ValueDoNotFree is no longer
													needed.
		 <2>	 3/31/94	EL		Export cmFreeProperties. #1150214
		 <1>	  2/3/94	EL		first checked in
		 <2>	  1/6/94	EL		Add ValueDoNotFree to value flag.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                            <<<  TOCEnts.h   >>>                           |
 |                                                                           |
 |                   Container Manager TOC Entry Interfaces                  |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/02/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file contains the interfaces to the manipulation routines to handle TOC objects
 below the object level.  The objects and the means to get at them are maintained in
 TOCObjects.c.  The split is done this way to keep the accessing of objects separate from
 the stuff represented by those objects.  See  TOCObjs.c   for complete details.
*/

#ifndef __TOCENTRIES__
#define __TOCENTRIES__


#include <setjmp.h>
#include <stdio.h>

/*------------------------------------------------------------------------------------*
 | W A R N I N G - - -> this header as well as  TOCObjs.h   and  TOCEnts.h   all have |
 |                      mutual references.  The include order is NOT arbitrary.  It's |
 |                      the only one that works!  Even with it we have to do some     |
 |                      forward struct pointer references of the form "struct foo *p" |
 *------------------------------------------------------------------------------------*/

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __DYNAMICVALUES__
#include "DynValus.h"     
#endif
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __UPDATING__
#include "Update.h"  
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
#endif

struct TOCProperty;
struct TOCValueHdr;
struct DynValueHdrExt;

																 CM_CFUNCTIONS

														/*-------------------*
														 | Values (segments) |
														 *-------------------*

                                W A R N I N G !!

This union may be the source of portability problems on machines that don't have 4-byte
longs, 2-byte shorts, and 1-byte bytes.  Byte ordering may also lead to portability		
problems.																																								*/

union TOCValueBytes {									/* Layout of value/length fields:									*/
	struct {														/* 		value if not immediate(not explicitly here):*/
		CM_ULONG value;										/*				offset to value if not immediate				*/
		CM_ULONG valueLen;								/*				value length if not immediate 					*/
	} notImm;
	
	struct {														/* 		value for a global name											*/
		CM_ULONG 					offset;					/*				offset to value in container						*/
		struct GlobalName	*globalNameSymbol;/*			ptr value for a global name (in memory)	*/
	} globalName;
	
	union {															/*		actual value if immediate placed here:			*/
		CM_UCHAR 	ucharsValue[2*sizeof(CM_ULONG)];/*value if immediate unsigned char(s)			*/
		CM_ULONG 	ulongValue;							/*				value if immediate unsigned long				*/
		CM_USHORT ushortValue;						/*				value if immediate unsigned short				*/
		CM_UCHAR	ubyteValue;							/*				value if immediate unsigned byte				*/
		void	 	 	*ptrValue;							/*				value if immediate pointer							*/
	} imm;
};
typedef union TOCValueBytes TOCValueBytes, *TOCValueBytesPtr;

struct TOCValue {											/* Layout of a TOC type's value:									*/
	ListLinks	 	   		 valueLinks;			/*		links to next/prev value 		(must be first)	*/
	struct TOCValueHdr *theValueHdr;		/*		ptr back to ValueHdr "owning" this value		*/
	ContainerPtr 	 		 container;				/*		ptr to "owning" container control block			*/
	CMValueFlags   		 flags;						/*		flags																				*/
	TOCValueBytes  		 value;						/*		value and length or immediate value					*/
	CM_ULONG					 logicalOffset;		/*		original (unedited) logical offset					*/
};
typedef struct TOCValue TOCValue, *TOCValuePtr;

enum ConstValueType {									/* Data types used to copy data into TOCValue's:	*/
	Value_NotImm,												/*		not immediate    ==> value and valueLen			*/
	Value_GlobalName,										/*		global name ptr  ==> in-memory str ptr 			*/
	Value_Imm_Chars,										/*		immediate, chars ==> ucharsValue						*/
	Value_Imm_Long,											/*		immediate, long	 ==> ulongValue							*/
	Value_Imm_Short,										/*		immediate, short ==> ushortValue						*/
	Value_Imm_Byte											/*		immediate, byte  ==> ushortValue						*/
};
typedef enum ConstValueType ConstValueType;

/* Note: The "owning" container control block of a value segment may not necessarily be	*/
/*			 the same as the one that owns the value header of the segments (shwon below).	*/
/* 			 The owner of the value header is always the container control block associated	*/
/* 			 with the TOC for that container.  The owner of a value segment, however, may		*/
/* 			 be a DIFFERENT TOC due to one container "editing" another.	 Editing is also		*/
/* 			 the reason for the originalOffset field in value segments.  Using it we can		*/
/*			 determine inserts, deletes, and overwrites.																		*/


															/*---------------*
															 | Value Headers |
															 *---------------*/
															 
struct TOCValueHdr {									/* Layout of a TOC property type:									*/
	ListLinks	 		 	 	 	valueHdrLinks;	/*		links to next/prev value hdr (must be first)*/
	struct TOCProperty 	*theProperty;		/*		ptr to "owning" property										*/
	ListHdr		 		 	 	 	valueList;			/*		list of actual values												*/
	CMObjectID 		 	 	 	typeID;					/*		the value's type ID													*/
	ContainerPtr 	 			container;			/*		ptr to "owning" container control block			*/
	CM_ULONG						size;						/*		total current size of the value data				*/
	CM_ULONG					 	logicalSize;		/*		original (unupdated) size of the value data	*/
	CM_USHORT			   	 	valueFlags;			/*		flags indicating stuff about the value			*/
	CMGeneration   		 	generation;			/*		generation number														*/
	CM_ULONG			  	 	useCount;				/*		count of nbr of times "used"								*/
	CMRefCon 					 	valueRefCon;		/*		user's value refCon													*/
	TouchedListEntryPtr touch;					/* 		ptr to updating touched list entry					*/
	union {															/* 		this field depends on kind of value hdr:		*/
		struct TOCValueHdr 		*dynValue;	/*				ptr to dynamic value hdr or NULL				*/
		struct DynValueHdrExt *extensions;/*				ptr to dynamic value hdr extensions			*/
	} dynValueData;											/*		[extensions only when it's a dynamic value]	*/
	union {															/*		references recorded by this value						*/
		TOCObjectPtr refDataObject;				/*				associated ref object; NULL if no refs	*/
		ListHdrPtr 	 refShadowList;				/*				or shadow list of the actual data				*/
	} references;												/*				(refShadowList only in recording value)	*/
};
typedef struct TOCValueHdr TOCValueHdr, *TOCValueHdrPtr;

struct DynValueHdrExt {								/* Extensions to TOCValueHdr for a dynamic value:	*/
	TOCValueHdrPtr  	 baseValue;				/*		ptr to base value of this dynamic value			*/
	DynamicValueVector dynValueVector;	/*		dynamic value handler vector								*/
	CMMetaHandler			 metaHandler;			/*		metahandler to get handler addresses				*/
};
typedef struct DynValueHdrExt DynValueHdrExt, *DynValueHdrExtPtr;

/* Some of following valueFlags echo the flags field a TOCValue entry. That is because 	*/
/* a CMValue "refNum" that an API user is given and in turn given back to us is a 			*/
/* pointer to a TOCValueHdr.  It is sometimes more convenient therefore to check the 		*/
/* kind of value we have by looking at the header then "going out" to the value. In all */
/* but continued values there is only one TOCValue entry on the valueList anyway.  So 	*/
/* echoing is more efficient than always going after the tail or head (they're the same)*/
/* of a valueList just to see the flags and the kind of value.													*/

#define	ValueDeleted			0x0001U			/* valueFlags: 1 ==> deleted value								*/
#define ValueContinued		0x0002U			/*						 	 ==> continued										*/
#define ValueGlobal				0x0004U			/*							 ==> global name									*/
#define ValueImmediate		0x0008U			/*							 ==> immediate value							*/
#define ValueOffPropChain	0x0800U			/*							 ==> dynamic value off prop chain	*/
#define ValueDynamic			0x1000U			/*							 ==> dynamic value								*/
#define ValueUndeletable	0x2000U			/*							 ==> can't be deleted 						*/
#define ValueProtected		0x4000U			/*							 ==> locked/protected value				*/
#define ValueDefined			0x8000U			/*							 ==> fully defined (in read only)	*/

/* Note: ValueUndeletable and ValueProtected are levels of protection bits. They protect*/
/* 			 a value from certain operations like writing to them or deleting them. The			*/
/*			 ValueProtected is a superset of ValueUndeletable.  ValueUndeletable says just 	*/
/*			 than ond only that, i.e., you can do anything to it except delete it.					*/

#define ValueCstFlags (ValueDefined   	| /* These flags are generally set in various 	*/\
											 ValueProtected 	| /* places and must remain set when we do the	*/\
											 ValueUndeletable | /* standard echoing of the value flags in			*/\
											 ValueDynamic 		| /* cmSetTOCValueHdrFlags() in TOCEntries.c.		*/\
											 ValueOffPropChain|	/* This mask is used there.										*/\
											 ValueContinued)
											 
/* In order to make dealing with dynamic values easier, the following macros are				*/
/* provided.  One is a more self-documented test to see if a TOCValueHdr is indeed a 		*/
/* dynamic value, while the other allows simpler notational access to a dynamic value		*/
/* header's extension fields.																														*/

#define IsDynamicValue(v)	((((TOCValueHdrPtr)(v))->valueFlags & ValueDynamic) != 0)

#define DYNEXTENSIONS(v) 							/* to make access to extensions a "little" easier!*/\
													 (((TOCValueHdrPtr)(v))->dynValueData.extensions)


																	/*------------*
																	 | Properties |
																	 *------------*/

struct TOCProperty {									/* Layout of a TOC object property:								*/
	ListLinks	 	 propertyLinks;					/*		links to next/prev property (must be first)	*/
	TOCObjectPtr theObject;							/* 		ptr to "owning" object											*/
	CMObjectID 	 propertyID;						/*		the property's ID		 												*/
	ListHdr			 valueHdrList;					/* 		list of the property's values								*/
};
typedef struct TOCProperty TOCProperty, *TOCPropertyPtr;
		
	
CM_USHORT cmSetTOCValueHdrFlags(TOCValueHdrPtr theValueHdr, const CM_USHORT flags);
	/*
	This routine is used to echo the settings of a value's flags in the value header which
	"owns" the value (TOCValue's are on a list whose header is in a TOCValueHdr).  This is
	done because a CMValue "refNum" that an API user is given and in turn given back to us
	is a pointer to a TOCValueHdr.  It is sometimes more convenient therefore to check the
	kind of value we have by looking at the header then "going out" to the value. In all but
	continued values there is only one TOCValue entry on the valueList anyway.  So echoing
	is more efficient than always going after the tail or head (they're the same) of a
	valueList just to see the flags and the kind of value.	
	
	The function updates the valueFlags field in the passed TOCValueHdr according to the
	value flags also passed.  The updated valueFlags are also returned as the function
	result.
	*/
	

TOCValueBytesPtr cmSetValueBytes(const ContainerPtr container,
																 TOCValueBytesPtr theValueBytes,
															 	 ConstValueType dataType, CM_ULONG data,
															 	 CM_ULONG dataLength);
	/*
	This routine is used to just set the value (theValueBytes) with the specified data. The
	data is placed in theValueBytes as a function of the specified type, dataType.  There
	are 7 types:
	 
	(1). Value_NotImm				This is the "normal" case for a non-immediate value data (a
													container offset) and data length.  The value/valueLen (i.e.,
													notImm) variant of a TOCValueBytes is used.
													 
	(2). Value_Imm_Chars    All of these use the corresponding imm variant of the ptr to the
	(3). Value_Imm_Long     TOCValueBytes struct passed here.  The data is placed in the
	(4). Value_Imm_Short    field according to type and, for the fun of it, the notImm
	(5). Value_Imm_Byte		  valueLen field set accordingly.  Note signed and unsigned are
	(6). Value_Imm_Ptr      treated alike.
													 
	(7). Value_GlobalName		This is a special case for global name strings. The data is
													assumed to be a pointer to a global name string.  A global name
													symbol table entry is created (using cmDefineGlobalName()) with
													the string.  The pointer to the symbol table entry is set as
													the value data. Note, a global name symbol table entry also
													has a back pointer to its "owning" TOCValue. THE CALLER MUST
													SET THIS FIELD!  Here we only deal with the value bytes.
													 
	The function returns the input theValueBytes pointer as its result.  NULL is returned
	for Value_GlobalName if the allocation fails and an error will have been reported.
	*/


TOCValuePtr cmCreateValueSegment(TOCValueHdrPtr theValueHdr, TOCValueBytesPtr value,
																 const CM_USHORT flags);
	/*
	This routine is called to create a new value segment and to fill in its fields with the
	passed parameters.  A pointer to the newly created value segment is returned.  NULL is
	returned if there is an allocation error.
	
	Segments are created generally created for continued values and appended on to the value
	chain whose header is container in a value header.  However, due to such things like
	value inserts a value segment may not necessarily be appended to the end of its chain.
	Thus this routine, which brings a value segment into existence, is a separate routine.
	
	Note, only the segment is created here.  It is up to the caller to chain it to its value
	header and echo the flags in that header.
	*/
	

TOCValuePtr cmAppendValue(TOCValueHdrPtr theValueHdr, TOCValueBytesPtr value,
													const CM_USHORT flags);
	/*
	This routine takes a pointer to a value definition (the actual bytes) and its flags
	and creates a TOCValue struct which is appended to the specified value header.  The
	function returns the value pointer as its result (i.e. pointer to the newly created
	value).  NULL is returned and an error reported if the allocation of the TOCValue fails.
	
	This routine is used to append actual values to an existing value header.  It is allowed
	to call cmDefineObject() with a NULL value bytes pointer.  In that case an object is 
	created with a value header but no value sublist off of that header.  This routine can
	be called to build that sublist.
	*/
	

TOCObjectPtr cmDefineObject(ContainerPtr container, CM_ULONG objectID,
									   				const CM_ULONG propertyID, const CM_ULONG typeID, 
										 				TOCValueBytesPtr value, const CM_ULONG generation,
														const CM_USHORT flags, const CM_USHORT objectFlags,
														TOCValueHdrPtr *theValueHdr);
	/*
	This routine is called to define a new TOC entry for an object.  We may have either a
	new object (id) or a preexisting one for which a new property and value are to be 
	defined.  All the fields for a TOC entry are passed.  The objectFlags indicate the
	type of the object (more about this later).
	
  Note, the value associated with the property is appended to the the end of the
	property's value list.  The caller has the option of getting the pointer to the value
	header (theValueHdr) when theValueHdr is not passed as NULL.
	
	The function returns a pointer to the object if it was successfully created and NULL if
	it wasn't.  An error is reported if NULL is returned.  Note, while error calls are not
	supposed to return we assume here they due just to be safe!
	
	The objectFlags determine how we treat the object and all the object fields (the other
	parameters).  There are four possible objectFlags:
	
	(1). UndefinedObject		Set if the object is to be created, but we don't yet know what
													its TOC entries are.  Basically a null object (or placeholder)
													is created.  It is an incomplete object in that there are no
													properties or types chained to the object (yet).  This flag may
													be used in combination with the others if we know that the 
													object ID corresponds to a property, type, or neither.
													 
	(2). ObjectObject				This flags is used when we don't know the type of the object but
													we know a property and type for it.  If the object already exists
													and it's undefined (UndefinedObject) it is now considered as
													defined.  If it was an undefined property or type, it now becomes
													a defined property or type.  Of course, duplicate definitions
													are an error.
	
	(3). PropertyObject			This is similar to ObjectObject, but here we know the object is
													for a property.  It has to either not exist previously, or was
												  previously flagged as UndefinedObject and PropertyObject.
	
	(4). TypeObject					Same as PropertyObject, but for type objects.
 
	This routine is the "main control" of Container Manager object creation.  It is used
	for "normal" object creation and while loading in a TOC from a preexisting container
	(i.e., from loadTOC()). After loadTOC(), or if we are writing and never do a loadTOC()
	we have the "normal" case.  "We control the horizontal. Do not adjust your TV set". We
	always know what we are creating (yeah, I got a bridge to sell you too).  So we never
	have undefined entries.  Thus in that case we can link the objects to their master lists. 
 
	For loadTOC() however, we are creating objects as we see thier id's in each TOC entry as
	we read them.  We can have forward references to undefined and yet to be fully
	created objects and backward references to existing objects.  We can also have multiple
	properties and values for the same object.  Hence the flags and all they imply.  We also
	do not link the objects.  That is delayed until the TOC is completely read in.  At that
	point the entire TOC is walked sequentially and the links built.  We can also do some
	additional validation to make sure that everything is defined after the read in.
	*/


void cmMarkValueDeleted(ContainerPtr container, TOCValueHdrPtr theValueHdr,
												CMBoolean deleteHdr);
	/*
	This routine takes a pointer to a value header in the specified container and deletes it.
	If the value was the only one for its property, the property is also deleted.
	
	Note, properties and values actually have their memory freed up.  The value headers are
	placed on the deletedValues list to keep them around, but out of the TOC.  This is to
	prevent dangling references from API user "refNum"s, because it is value header pointers
	that we give to the user as those "refNum"s.  Such value headers are marked as "deleted"
	to prevent future use by the user.
	
	The moving of the deleted value headers to the deletedValues list can be suppressed by 
	passing the deleteHdr parameter as true.  Its memory is simply freed along with the rest.
	This is used currently in only one place for error recovery when dynamic value setup
	fails.  There we want to free all the dynamic value layers (which are headers), and
	finally the base "real" value itself.  It is that final value that is passed to here to
	get rid of.
	*/


void cmDeleteProperty(ContainerPtr container, TOCPropertyPtr theProperty);
	/*
	This routine takes a pointer to a property in the specified container and deletes it and
	all its values.
	
	Note, properties and values actually have their memory freed up.  The value headers are
	placed on the deletedValues list to keep them around, but out of the TOC.  This is to
	prevent dangling references from API user "refNum"s, because it is value header pointers
	that we give to the user as those "refNum"s.  Such value headers are marked as "deleted"
	to prevent future use by the user.
	*/


void cmMarkObjectDeleted(ContainerPtr container, TOCObjectPtr theObject);
	/*
	This routine is called to remove an entire object from the TOC for the specified
	container.  All its associated structures (properties and values) are made unavailable
	from the TOC.  The space for the object is NOT freed. The object is placed on a list of
	deleted objects and marked as deleted.  By doing it this way, any outstanding "refNum"s
	we gave the API user (which are pointers to this stuff) will still be "valid" (sort of
	-- at least not looking at garbage).
	*/
	
	
void cmFreeTOC(ContainerPtr container, void **toc);
	/*
	This routine is called to free all the structures in a TOC. For updating, a container may
	have its own TOC and the target TOC.  Thus the TOC pointer is an explicit parameter. It
  is passed as a pointer to the toc pointer because freeing the TOC will NULL out the
  caller's pointer.
	
	All memory for objects, properties, and types is freed.  On return the toc pointer in the
	container is NULL as well as all the master link head/tail pointers.
	*/


TOCPropertyPtr cmGetObjectProperty(TOCObjectPtr theObject, CM_ULONG propertyID);
	/*
  This routine takes a pointer to an object and scans its properties for the specified ID.
  The pointer to the property on the object's property chain is returned if found.  NULL is
  returned if not found.
	*/


TOCValueHdrPtr cmGetPropertyType(TOCPropertyPtr theProperty, CM_ULONG typeID);
	/*
	This routine takes a pointer to a object's property and scans its value headers for one
	containing the specified type ID.  The pointer to the value header is returned if found.
	NULL is returned if not found.
	*/
	
	
CM_ULONG cmGet1ValueSize(TOCValuePtr theValue);
	/*
	This routine takes a pointer to a value (NOT a value header -- continued values are not
	worried about here) and returns the size represented by that value.  For non-immediate
	values we simply return the length field from the value.  Immediate value sizes are
	dependent on their type.
	*/


#if CMVALIDATE
#define cmValidateCMObject(object) 		 cmValidateCMObjects((CMObject)(object),   0xFFFFU)
#define cmValidateCMProperty(property) cmValidateCMObjects((CMObject)(property), PropertyObject)
#define cmValidateCMType(type) 				 cmValidateCMObjects((CMObject)(type), 		 TypeObject)
CMBoolean cmValidateCMValue(CMValue value);
CMBoolean cmValidateCMObjects(CMObject object, CM_USHORT objectFlags);
	/*
	CMObject, CMProperty, CMType, and CMValue "refNum"s in the API are implemented in the
	Container Manager as pointers to TOCObject structs (CMObject, CMProperty, and CMType) and
	TOCValueHdr structs (CMValue).  These routines is used to filter "bad" object "refNum"s
	passed to the higher level API interface  ("CM...") routines.  False is returned if the
	object, property, type or value is considered as "bad" and true otherwise.
	
	As currently implemented we return false but do NOT report an error for NULL pointers.
	That is because we return NULL from everthing we error out of earlier.  The NULL test is
	just protection from letting such pointer from ever being used.
	
	Another check we do without reporting an error is on the container pointer.  The refNums
	have a pointer to their container.  The container has a special pointer pointing to 
	itself.  We check against this pointer.  If we think we don't have a valid container
	pointer then we assume we have a bad refNum and return false.  No error can be reported
	because, wiut a container pointer, there is no way to do it!
	
	We DO report an error for attempting to use a deleted object, property, type, or value.
	Deleted TOCObject's and TOCValueHdr's are NOT freed.  Rather they are put on a list
	(separate lists) of deleted objects or value headers and flagged as deleted just for the
	purpose of testing by these routines.  If we had freed them, then we would be getting
	pointers to "garbage".  By keeping them around we have valid pointers and we can do the
	test.
	
	For types and properties we also validate that such "refNum"s are indeed types or
	properties.
	
	Note, for objects, types, and properties, the macros defined above should be used.  They
	expand to calls to cmValidateCMObjects().
	*/
#endif
	
	
enum TOCWalkReturns {									/* cmWalkThroughEntireTOC() action return codes:	*/
	WalkNextTOCObject,									/* 		do next object															*/
	WalkNextTOCProperty,								/*						property														*/
	WalkNextTOCValueHdr,								/*						value header												*/
	WalkNextTOCValue										/*						value segment												*/
};
typedef enum TOCWalkReturns TOCWalkReturns;
	/*
	The WalkReturns are the return codes that each cmWalkThroughEntireTOC() action routine is
	to return to direct the walking through the TOC.  cmWalkThroughEntireTOC() is described
	below.  It takes action routines to apply to each data structure of the TOC as it is
	walked.  These return codes allow tighter control over the walking.
	
	The action routines should return the codes shown in the following table:
			
					  Walk...  NextTOCObject | NextTOCProperty | NextTOCValueHdr | NextTOCValue
		===============|===============|=================|=================|==============
		objectAction   |  next object  |  next property* |                 |
		propertyAction |  next object  |  next property  | next value hdr* |
		valueHdrAction |  next object  |  next property  | next value hdr  | next segment*
		valueAction    |  next object  |  next property  | next value hdr  | next segment*
		---------------+---------------+-----------------+-----------------+--------------
		
		* The entries marked with "*" are the returns that should be used in the normal case
		  to fully walk each data structure of the TOC.  Those not marked with "*" are for
			special cases to abort the walk to go to the next indicated data structure.  Walks
			are sequential through the TOC by ascending object ID.  Thus skipping a particular
			walk of one structure only makes sense if the skip is possible.  That is the reason
			not all returns are allowed in all action routines.  In reality, only the non-starred
			returns are checked on return from an action routine. So the default is the starred
			return (i.e., keep walking normally) if a non-starred is not returned.
		
	*/


int cmWalkThroughEntireTOC(ContainerPtr container, void *toc, 
													 CMObjectID startingID, CMObjectID endingID,
													 CMRefCon refCon,
												 	 TOCWalkReturns (*objectAction)(ContainerPtr container, TOCObjectPtr theObject, CMRefCon refCon),
												 	 TOCWalkReturns (*propertyAction)(ContainerPtr container, TOCPropertyPtr theProperty, CMRefCon refCon),
												 	 TOCWalkReturns (*valueHdrAction)(ContainerPtr container, TOCValueHdrPtr theValueHdr, CMRefCon refCon),
												 	 TOCWalkReturns (*valueAction)(ContainerPtr container, TOCValuePtr theValue, CMRefCon refCon));
	/*
	This routine is called to walk the entire specified TOC by ascending object ID, starting 
	with object ID's greater than or equal to the startingID and less than or equal to
	endingID, and to call an action routine appropriate for each kind of structure: object,
	property, value header, and value.  
	
	Pointers to the four action routines are passed with headers as shown in the definition. 
	If a pointer is passed as NULL, no action will be performed for the corresponding
	structure.  Each function has return codes (type TOCWalkReturns) that direct the walking
	through the TOC.	The action routines should return the codes shown in the following
	table:
			 
						 Walk...  NextTOCObject | NextTOCProperty | NextTOCValueHdr | NextTOCValue
		 ===============|===============|=================|=================|==============
		 objectAction   |  next object  |  next property* |                 |
		 propertyAction |  next object  |  next property  | next value hdr* |
		 valueHdrAction |  next object  |  next property  | next value hdr  | next segment*
		 valueAction    |  next object  |  next property  | next value hdr  | next segment*
		 ---------------+---------------+-----------------+-----------------+--------------
 
	The entries marked with "*" are the returns that should be used in the normal case to
	fully walk each data structure of the TOC.  Those not marked with "*" are for special
	cases to abort the walk to go to the next indicated data structure.  Walks are sequential
	through the TOC by ascending object ID.  Thus skipping a particular walk of one structure
	only makes sense if the skip is possible.  That is the reason not all returns are allowed
	in all action routines.  In reality, only the non-starred returns are checked on return
	from an action routine. So the default is the starred return (i.e., keep walking normally)
	if a non-starred is not returned.
	
	A "refCon" is also passed which the caller can use as a communication facility to convey
	additional info to the action routines.  
	
	This function returns 0 to indicate successful completion, and non-zero otherwise.  Thus
	if AbortWalkThroughEntireTOC(x) is used to abort processing, the x should be a positive
	non-zero integer.
	
	Note, if the property, value header, and value action routines are supplied as NULL,
	then cmWalkThroughEntireTOC() is functionally equivalent to cmForEachObject() which just
	applies a single action routine to the objects.  Thus if only the objects are to be
	walked, it is recommended that cmForEachObject() be used instead of
	cmWalkThroughEntireTOC().
	
	Note also, this routine uses cmForEachObject() to walk the TOC objects with walkObject()
	defined above as the action routine.  It handles the walking of the structures "below"
	the object level. We use the refCon parameter to cmForEachObject() to communicate to
	walkObject() all the action routine pointers and the caller's refCon.  In particular,
	the refCon is a pointer to the container.  In the container is a pointer BACK to a local
	data structure we set up here with all the info.
	*/


#define AbortWalkThroughEntireTOC(x) longjmp(*(jmp_buf *)container->tocActions, x)
	/*
	Use this in routines passed to cmWalkThroughEntireTOC() to abort the walk. The value of x
	is returned from cmWalkThroughEntireTOC(). It should be non-zero since 0 is the "success"
	return value for cmWalkThroughEntireTOC().
	*/


int cmWalkObject(ContainerPtr container, TOCObjectPtr theObject, CMRefCon refCon,
								 TOCWalkReturns (*objectAction)(ContainerPtr container, TOCObjectPtr theObject, CMRefCon refCon),
								 TOCWalkReturns (*propertyAction)(ContainerPtr container, TOCPropertyPtr theProperty, CMRefCon refCon),
								 TOCWalkReturns (*valueHdrAction)(ContainerPtr container, TOCValueHdrPtr theValueHdr, CMRefCon refCon),
								 TOCWalkReturns (*valueAction)(ContainerPtr container, TOCValuePtr theValue, CMRefCon refCon));
	/*
	This routine is called to walk a single object (theObject) and call an action routine
	appropriate for each kind of structure: object, property, value header, and value. 
	Pointers to the four action routines are passed with headers as shown in the definition.
	If a pointer is passed as NULL, no action will be performed for the corresponding
	structure.  The returns are the same as cmWalkThroughEntireTOC().  See it for further
	details.
	
	A "refCon" is also passed which the caller can use as a communication facility to convey
	additional info to the action routines. 0 is returned to indicate successfull completion.
	Use the AbortWalkObject(x) (a macro) in an action routine to abort the walk.  The "x" is
	a integer which is returned from cmWalkObject() so it should not be 0.
	*/


#define AbortWalkObject(x) longjmp(*(jmp_buf *)container->tocActions, x)
	/*
	Use this in routines passed to cmWalkObject() to abort the walk. The value of x is
	returned from cmWalkObject(). It should be non-zero since 0 is the "success" return value
	for AbortWalkObject().
	*/

void cmFreeProperties(TOCObjectPtr theObject, CMRefCon refCon);

/*

 This routine is called to unconditionally free all the properties for an
 objects and the values and other data structures associated with those properties.  It is
 done when an TOC is deleted, i.e, for cmFreeTOC().
 
 Note, here we can just free the properties and associated data structures.  We don't have
 to respect the fact that they are on a list whose header belongs to the the object.  That
 is because we know we are going to delete the object too.
 
 This routine differs from deleteProperties(), and cmDeleteProperty() in that we just free
 the data.  There is no list maintenance and we do not have to worry about moving deleted
 objects or value headers to their delete lists.
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmDestroyTOC().
*/

														  CM_END_CFUNCTIONS
#endif
