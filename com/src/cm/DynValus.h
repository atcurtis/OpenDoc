/* @(#)Z 1.5 com/src/cm/DynValus.h, odstorage, od96os2, odos29646d 96/11/15 15:27:04 (96/10/29 09:18:00) */
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
	File:		DynValus.h

	Contains:	Container Manager Dynamic Value Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <2>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                          <<<   DynValus.h    >>>                          |
 |                                                                           |
 |                Container Manager Dynamic Value Interfaces                 |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                  5/5/92                                   |
 |                                                                           |
 |                     Copyright Apple Computer, Inc. 1992-1994              |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 Dynamic values are special values which "know" how to manipulate on their own value data.
 They do this through a set of value operation handlers which are expected to be
 semantically the same as the standard API value operations.
 
 The following value operations are expected to be supported by these handlers:
 
 CMSize CMGetValueSize(CMValue value);
 CMSize CMReadValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize maxSize,
 												CMType *type);
 void CMWriteValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize size);
 void CMInsertValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize size);
 void CMDeleteValueData(CMValue value, CMCount offset, CMSize size);
 void CMGetValueInfo(CMValue value, CMContainer *container, CMObject *object,
										 CMProperty *property, CMType *type, CMGeneration *generation);
 void CMSetValueType(CMValue value, CMType type);
 void CMSetValueGeneration(CMValue value, CMGeneration generation);
 void CMReleaseValue(CMValue);

 Whenever a dynamic value is passed to a standard API value routine corresponding to the
 above handlers, the handler is called instead.  That is why the handler must be
 semantically identical.
 
 Basically, dynamic values are similar to C++ objects where the handlers are the object's
 methods.  They are dynamic in the sense that they only exist from creation (discussed
 below) and last until until they are released (CMReleaseValue()).
 
 A dynamic value can have its own data (similar to C++ class fields).  The data is
 generally used to create the handler's refCon during dynamic value creation.
  
 A dynamic value results when a value is created by to CMNewValue() or attempted to be
 used by CMUseValue(), and the following two conditions occur:
 
 	1. The type or any of its base types (if any, created by CMAddBaseType()), are for
 		 global names that have associated metahandlers registered by CMRegisterType().
		 
	2. The metahandlers support the operation type to return a "use value" handler, and
		 in addition for CMNewValue(), a "new value" handler.

 The "new value" handlers are used to define initialization data for the "use value"
 handlers.  The "use value" handlers are called to set up and return a refCons.  Another
 metahandler address is also returned. This is used to get the address of the value
 operation handlers corresponding to the standard API CM... value routines mentioned above.
 
 Just like C++, dynamic values may be "subclassed" via their (base) types.  If a handler
 for a particular operation is not defined for a value, its "base value" is used to get the
 "inherited" handler.  This continues up a dynamic value's chain of base values, up to the
 original "real" value that spawned the base values from the CMNewValue() or CMUseValue().
 
 Thus for each dynamic value, there is a vector of handler addresses.    DynValus.h   
 defines the layout of that vector.  Also defined there are the other interfaces needed to
 process a dynamic value by the API routines themselves.  The routines themselves are in
 this file.

 The vector becomes part of a set of "extensions" to the dynamic value header.  The
 extensions struct is defined in  TOCEnts.h   along with the other value header
 definitions.  The dynamic value stuff is kept separate because it is sufficiently
 distinct.
 
 For a full description of the dynamic value data structures and how dynamic values work
 see DynamicValues.c.  It's "everything you wanted to know about dynamic values and
 afraid to ask" kind of description.
  
 Implementation note: This header is slightly different from the other headers in its
 											organization.  This is due to the complexity involved with dynamic
											values.  The various declarations are ordered such that this reads
											as a description from top to bottom.  They are not just as set of
											isolated (but related) definitions which is generally the case in
											the other headers.  So this file is best understood by reading it 
											in the order presented.
*/

#ifndef __DYNAMICVALUES__
#define __DYNAMICVALUES__

#include <stdarg.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif

struct Container;
struct TOCObject;
struct TOCValueHdr;

																	CM_CFUNCTIONS

/* The following defines what each entry in the dynamic value vector looks like...			*/

struct DynamicValueVectorEntries {		/* Layout of a dynamic value vector entry:				*/
	CMHandlerAddr handler;							/*		the handler address													*/
	CMValue				thisValue;						/*		the handler's value (C++ "this")						*/
	CMBoolean 		active;								/*		true ==> handler is in calling chain				*/
};
typedef struct DynamicValueVectorEntries DynamicValueVectorEntries, *DynamicValueVectorEntriesPtr;

/* When a handler is called, it is expected to do its operations on the "base value" of	*/
/* the value passed to it.  It gets its base value using CMGetBaseValue().  However,		*/
/* we don't want to allow recursive use of the API for the same value.  That would			*/
/* call the handler again and we would be in an infinit loop!  Thus the active switch		*/
/* is provided to check for this so we can report an error.															*/

/* The vector is initialized with each handler address thisValue set to NULL.  On first */
/* use we use the methandler returned from the "use value" handler (saved in the value 	*/
/* header extensions) to get the proper handler address.  It is saved in the handler 		*/
/* field of the vector entry. Remember we may have to search up through a dynamic value */
/* chain to find an "inherited" method ('er, excuse me, value handler operation).  Thus */
/* the handler used may correspond to a different dynamic value. We must therefore save */
/* the dynamic value refNum along with the handler address (in the thisValue).  It is 	*/
/* basically the C++ "this" pointer for the method (oops, sorry again, value handler 		*/
/* operation).																																					*/

/* Of course, in the simplest case, where the handler is provided for the original 			*/
/* value, the thisValue will point to its own dynamic value header.  At the other				*/
/* extreme no handlers are supplied for the operation and we end up using the "real"		*/
/* value that spawned the dynamic value(s).  In that case the handler pointer in the		*/
/* vector entry remains NULL and the thisValue will be the "real" value refNum.  With		*/
/* no handler we use the actual API routine to process the real value.									*/

/* Now that you understand what each vector entry contains, we can define the vector		*/
/* itself...																																						*/

struct DynamicValueVector {
	DynamicValueVectorEntries cmGetValueSize;
	DynamicValueVectorEntries cmReadValueData;
	DynamicValueVectorEntries cmWriteValueData;
	DynamicValueVectorEntries cmInsertValueData;
	DynamicValueVectorEntries cmDeleteValueData;
	DynamicValueVectorEntries cmGetValueInfo;
	DynamicValueVectorEntries cmSetValueType;
	DynamicValueVectorEntries cmSetValueGen;
	DynamicValueVectorEntries cmReleaseValue;
};
typedef struct DynamicValueVector DynamicValueVector;

/* Remember this becomes part of the extensions belonging only to a dynamic value header*/
/* as defined in TOCEntries.h.																													*/

/* Just as with standard handlers, to make it easier and more readable for the Container*/
/* Manager to call the dynamic value handlers, we will use macros.  These macros will		*/
/* require the following typedefs as casts to convert the generic handler typedef, 			*/
/* HandlerAddr (the type used to store the addresses in the vector), to the actual			*/
/* function type:																																				*/

typedef CMSize (*TcmGetValueSize)(CMValue value);
typedef CMSize (*TcmReadValueData)(CMValue value, CMPtr buffer, CMCount offset, CMSize maxSize);
typedef void (*TcmWriteValueData)(CMValue value, CMPtr buffer, CMCount offset, CMSize size);
typedef void (*TcmInsertValueData)(CMValue value, CMPtr buffer, CMCount offset, CMSize size);
typedef void (*TcmDeleteValueData)(CMValue value, CMCount offset, CMSize size);
typedef void (*TcmGetValueInfo)(CMValue value, CMContainer *container, CMObject *object,
																CMProperty *property, CMType *type, 
																CMGeneration *generation);
typedef void (*TcmSetValueType)(CMValue value, CMType type);
typedef void (*TcmSetValueGen)(CMValue value, CMGeneration generation);
typedef void (*TcmReleaseValue)(CMValue);

/* Note, these casts are not just placed here for the fun of it!  They are delibertly	*/
/* placed in front of the macros which will use them!  Why?  It seems some compilers		*/
/* don't support ANSI correctly (as I understand it) and require this. The c89 compiler	*/
/* on the RS/6000 is one such compiler.  It reported errors if the casts followed the		*/
/* macros (sounds like a bug to me)!																										*/

/* Here now are the macros used to do the calls using the vector...											*/

#define CMDynGetValueSize(v)							(*(TcmGetValueSize)DYNEXTENSIONS(v)->dynValueVector.cmGetValueSize.handler)((CMValue)(v))
#define CMDynReadValueData(v, b, x, m)		(*(TcmReadValueData)DYNEXTENSIONS(v)->dynValueVector.cmReadValueData.handler)((CMValue)(v), (CMPtr)(b), (CMCount)(x), (CMSize)(m))
#define CMDynWriteValueData(v, b, x, n)		(*(TcmWriteValueData)DYNEXTENSIONS(v)->dynValueVector.cmWriteValueData.handler)((CMValue)(v),	(CMPtr)(b), (CMCount)(x), (CMSize)(n))
#define CMDynInsertValueData(v, b, x, n)	(*(TcmInsertValueData)DYNEXTENSIONS(v)->dynValueVector.cmInsertValueData.handler)((CMValue)(v), (CMPtr)(b), (CMCount)(x), (CMSize)(n))
#define CMDynDeleteValueData(v, x, n)			(*(TcmDeleteValueData)DYNEXTENSIONS(v)->dynValueVector.cmDeleteValueData.handler)((CMValue)(v), (CMCount)(x), (CMSize)(n))
#define CMDynGetValueInfo(v,c,obj,p,t, g) (*(TcmGetValueInfo)DYNEXTENSIONS(v)->dynValueVector.cmGetValueInfo.handler)((CMValue)(v), (CMContainer*)(c), (CMObject*)(obj),\
																																																										(CMProperty*)(p), (CMType*)(t), (CMGeneration*)(g))
#define CMDynSetValueType(v, t)			 	 		(*(TcmSetValueType)DYNEXTENSIONS(v)->dynValueVector.cmSetValueType.handler)((CMValue)(v), (CMType)(t))
#define CMDynSetValueGen(v, g)			 			(*(TcmSetValueGen)DYNEXTENSIONS(v)->dynValueVector.cmSetValueGen.handler)((CMValue)(v), (CMGeneration)(g))
#define CMDynReleaseValue(v)							(*(TcmReleaseValue)DYNEXTENSIONS(v)->dynValueVector.cmReleaseValue.handler)((CMValue)(v))

/* If nothing else, the above shows that, with enough casts, you can get away with 			*/
/* anything in C!  Aren't casts wonderful?  Repeat after me -- casts are your friend.		*/

/* As mentioned earlier, each corresponding API value operation must check to see if it	*/
/* has a dynamic value and call the corresponding handler which does the operation. It	*/
/* must get the proper address on first use.  It must set switches to mark the handler	*/
/* as active.  It must also set a switch to allow CMGetBaseValue() calls which are only	*/
/* allowed from dynamic value handlers.  Thus the algorithm looks something like this 	*/
/* (ignoring all errors for the moment):																								*/

/*	if (IsDynamicValue(v)) {																														*/
/*		v = GetDynHandlerAddress(v, h, g);																								*/
/*		if (IsDynamicValue(v)) {																													*/
/*			SignalDynHandlerInUse(v, h);																										*/
/*			AllowCMGetBaseValue(container);																									*/
/*																																											*/
/*			Call the proper dynamic value handler with one of the above macros definitions.	*/
/* 			The macro will pass the appropriate value corresponding to a possibly inherited	*/
/* 			handler.  If the handler returns a value save it to be returned as the result.	*/
/*																																											*/
/*			DisAllowCMGetBaseValue(container);																							*/
/*			SignalDynHandlerAvailable(v, h);																								*/
/*			return [result];																																*/
/*		}																																									*/
/*	}																																										*/

/* In the above algorithm, 																															*/
/*		v is a dynamic CMValue (note that GetDynHandlerAddress may CHANGE it), 						*/
/*		h is a pointer to a vector entry in the extensions, 															*/
/*		g is the metahandlers operation type string,																			*/

/* The GetDynHandlerAddress() takes a vector entry and sets the handler address as a		*/
/* function of the "g" metahandler operation code.  On first call it will search for the*/
/* inherited method if necessary.	 The vector is updated with the found handler address	*/
/* and the associated "this" value saved.  This is the value returned and whose switches*/
/* we set.  We do the call and reset the switches, all using the same "this" value.			*/

/* Note, in the limit, the search for the inherited handler may end up finding the 			*/
/* original "real" value that spawned the dynamic values!  Thus what is returned from		*/
/* GetDynHandlerAddress() in this case is a "real" value to be operated upon using the	*/
/* standard API routine itself.  That is why a second IsDynamicValue() must be done to	*/
/* bypass the handler call and switch setting/resetting.																*/

/* Be careful here!  Since there is no guarantee the value coming back from the					*/
/* GetDynHandlerAddress() call is the one that went in, the code must reload any copies	*/
/* of the value sometime following the GetDynHandlerAddress() calls. This is highlighted*/
/* as something to look out for because it has already caused one bug!									*/

/* All of the above is what each corresponding API value operation must do.  Given all 	*/
/* this, it is impossible to write a generalized macro to minimize the coding. We could */
/* write a unique macro for each handler just as the vector calls are done above, but	 	*/
/* that would probably be confusing. 																										*/

/* So as a comprimise, the following macros are defined.  They basically mimic the			*/
/* above algorithm, but here take errors into account.																	*/

#define GetDynHandlerAddress(v, h, g, s, x) if ((v = cmGetDynHandlerAddress((CMValue)(v), &DYNEXTENSIONS(v)->dynValueVector.h, (CMGlobalName)(g), s)) == NULL) \
																							return x
#define SignalDynHandlerInUse(v, h)				 	DYNEXTENSIONS(v)->dynValueVector.h.active = true
#define SignalDynHandlerAvailable(v, h)		 	DYNEXTENSIONS(v)->dynValueVector.h.active = false
#define AllowCMGetBaseValue(container)			++((ContainerPtr)(container))->getBaseValueOk
#define DisAllowCMGetBaseValue(container)		if (--((ContainerPtr)(container))->getBaseValueOk < 0) \
																							((ContainerPtr)(container))->getBaseValueOk = 0;

/* The IsDynamicValue() call is defined in TOCEntries.h.																*/

/* The "h" in all these macros is the vector entry name.																*/

/* The "x" parameter in GetDynHandlerAddress has the same purpose as in    ErrorRpt.h   */
/* where we return CM_NOVALUE, NULL, or whatever is appropriate to indicate failure of	*/
/* the API routine.																																			*/

/* Also for GetDynHandlerAddress, the "s" is the name of the API routine doing the call.*/
/* This is used by cmGetDynHandlerAddress() simply to use as an insert if it should 		*/
/* report an error prior to returning NULL.																							*/

/* Note the AllowCMGetBaseValue and DisAllowCMGetBaseValue macros. As mentioned earlier,*/
/* a CMGetBaseValue() is only allowed from value operation handlers.  The two macros		*/
/* control a single switch, getBaseValueOk, the CMGetBaseValue() checks.  The switch is	*/
/* actualy a counter which, just to be safe, is never allowed to stay negative.  0 means*/
/* CMGetBaseValue() is illegal.  Greater than 0, it's legal.  The reason the switch is a*/
/* counter is because dynamic values use CMGetBaseValue() to do their operations in 		*/
/* terms of their base values. If a dynamic value's base is also dynamic (i.e., we have */
/* layered dynamic values), then we have a nesting condition.  Hence the counter.				*/

/* The rest of this header is more "normal", i.e., a collection of related definitions	*/
/* and function headers.																																*/

	
struct TOCValueHdr *cmFollowTypes(struct TOCValueHdr *theBaseValueHdr,
																	struct TOCObject *type,
																	CMBoolean isNewValue, va_list *constructorData);
	/*
	This routine creates a dynamic value layers for the passed type and all of its base types,
	if any of these types have a "use value" handler.  This routine is only called by
	CMNewValue() or CMUseValue().  For CMNewValue(), "metadata" handler and "new value"
	handlers are also required.  The top-most dynamic value header pointer is returned.  This
	should be returned from CMUseValue() or CMNewValue().  NULL is returned if an error is
	reported.  The original "real" value is returned if no dynamic values are created.
	 
	For CMUseValue(), isNewValue should be set to false.  It should only be set to true for
	CMNewValue().  Also for CMNewValue(), the constructorData must point at the CMNewValue()
	"..." parameters.  These are consumed as the base type metadata (returned from the 
	"metadata" handler) describes how to create data packets from the "..." parameters.  The
	packets, in turn, are passed to the "new value" handlers.  A "new value" handler uses its
	data packet to write (possibly different) data to its base value.  This written data will
	then be read and used by the "use value" handler.
	
	The "use value" handler is called for both the CMUseValue() and CMNewValue() cases.  If
	it's companion "new value" handler wrote data to its base value, the "use value" handler
	will probably read the data to create its refCon.  The refCon will be passed to all
	value handlers.  The "use value" handler returns its refCon along with another
	metahandler address that is used to get the value handler addresses.  These are then
	used to create the dynamic value.
	
	See dynamic value documentation in   DynValus.c    for a full a description of the
	prototypes for the "metadata", "new value", and "use value" handlers, as well as the
	description of the metadata itself.
	
	To produce all the required dynamic values, cmFollowTypes() recursively follows the
	types, looking for base types as defined by CMAddBaseType().  Each type can have any
	number of base types.  The recursion effectively produces a depth-first search of all
	the base types.
	
	As each type is completed (i.e., no more base types for it), a dynamic value is created
	as described above.  That is, for CMNewValue(), a type's "metadata" handler instructs us
	on how many CMNewValue() "..." parameters to consume and how to construct their packet.
	That is passed to the "new value" handler so it can write some appropriate data to the
	base value.  The "use value" is called in all cases which reads the data written by "new
	value" to construct its refCon.  The refCon is returned here along with the metahandler
	address that will yield the value handler routine addresses.
	
	The refCon and metahandler address are passed to newDynamicValue() to construct one
	dynamic value (layer).  The resulting dynamic value is used as the base value for the
	next layer.  This produces the desired data structures.
	
	Note, because this routine searches through the types down to their leaves, and then
	generates the dynamic values on the way back "up", the CMNewValue() "..." parameters
	must be ordered for the "deepest" type first.  For example, given the following type
	inheritance heirarchy (read these as T1 inherits from T2 and T3, and so on):
	
                                      T4      T5
                                        *    *
                                         *  *
                                  T2      T3
                                    *    *
                                     *  *
                                      T1
							 
	The depth-first search, starting at T1, yields the sequence: T2 T4 T5 T3 T1.  Then this
	is the order the CMNewValue() "..." parameters must be in.
	*/
	
	
CMValue cmGetDynHandlerAddress(CMValue value, DynamicValueVectorEntriesPtr vectorEntry,
															 CMconst_CMGlobalName operationType, char *routineName);
	/*
	A dynamic value handler is callable if it exists (of course!) and it is not being used
	recursively.  The vectorEntry points to the dynamic value entry in its vector belonging
	to the extensions of the passed dynamic value.  If this is not the first use of the
	handler, the vector entry contains the handler address and its associated ("this") value
	(discussed below).  If it is first use, cmGetDynHandlerAddress() must find the
	("inherited") handler address and its associated "this" value.
	
	No matter which case it is, the "this" value is returned as the function result.  NULL
	is returned if an error is reported (discussed later).
	 
	The returned value, and the one saved in the vector entry may not be the same.  They are
	if the passed value has a handler.  If it doesn't, an "inherited" handler, from one of
	the dynamic value's base values is used. The value associated for whoever has the handler 
	is the "owning" value.  In C++ terms, it is the "this" pointer.
	
	In the limit, we could end up using the original "real" value that spawned the dynamic
	value(s).  In that is indeed the case, we end up using the calling routine which will
	always be an API value operation (as shown in the algorithm above).
 
	The found handler address and "this" pointer are saved in the passed dynamic value's
	vector entry so we don't have to do the search on successive uses.  The "this" pointer
	and handler address are also saved in the vector entry corresponding to "this" (unless,
	of course, it is the "real" value).
	 
	In the vector entry there is also a boolean flag that tells us whether the handler is
	currently active, i.e., in the call chain.  If it is we have a recursion attempt.  This
	causes an error report and NULL to be returned.  Since we will always find the handler
	or use the "real" value, we can never get an error from that.  The recursion is the only
	error condition.
	*/

CM_ULONG cmVScanDataPacketGuts(struct TOCObject *type, CM_CHAR *metaData,
															 CM_UCHAR *dataPacket, va_list dataInitParams);
	/*
	This routine is the "guts" of the routine CMScanDataPacket().  It is used to extract the
	fields of a data packet created from CMNewValue() "..." parameters according to the
	specified metadata and place it in the "dataInitParams" parameters.  Each dataInitParams
	parameter must be a pointer; extracted data read from the data packet are stored into the
	locations pointed to by the pointers.
	
	The Container Manager accesses the metadata through a "metadata" handler for the type to
	build the data packet.  cmVScanDataPacket() inverts the operation and allows a "new
	value" handler to extract the data back into distinct variables. The "new value" handler
	can use its own "metadata" handler to pass to the CMScanDataPacket() routine to extract
	the data.
	
	The function returns the number of data items extracted and assigned to the parameters.
	This could be 0 if the type does not have any metadata (or if an error is reported).
	
	NULL may be passed as the dataPacket or metaData pointer to indicate there is no
	metadata.  
	
	The metadata format is discussed in DynamicValues.c.  The only special case is for the
	"%*s" format specification.  This generates a fixed length string (not null terminated)
	of n characters, where n is determined from a parameter in the CMNewValue() "..."
	paramters.  The value of n is explicitly written to the packet data in front of the
	string.  For symmetry (and so we can determine the number of characters to extract from
	the packet data), we return the value of n to an explict parameter pointer passed here.
	Thus the parameter pointer list passed to this routine should be identical to the "..."
	parameters passed to a CMNewValue() "..." parameter list (at least the portion
	corresponding to this type).
	*/


void cmDeleteAllDynamicValueLayers(struct TOCValueHdr *theValueHdr, CMBoolean freeValueHdr);
	/*
	This routine is used in two contexts; first, for error recovery to free all the memory
	for a newly allocated dynamic value layer set, and second, when freeing the entire TOC
	to make usre all memory for any "dangling" dynamic values are freed.
	
	The current top-most dynamic value layer is passed in theValueHdr.  The layers are
	back-linked up to the base "real" value.  All the dynamic value memory on this chain are
	freed.  If freeValueHdr is true, then we were creating dynamic values for CMNewValue().
	In that case, the real value itself is freed.  Really freed!  Not marked free as is the
	usual case.
	*/


														 CM_END_CFUNCTIONS
#endif
