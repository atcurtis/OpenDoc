/* @(#)Z 1.4 com/src/cm/DynValus.c, odstorage, od96os2, odos29646d 96/11/15 15:27:03 (96/10/29 09:17:53) */
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
	File:		DynValus.c

	Contains:	xxx put contents here xxx

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                          <<<   DynValus.c    >>>                          |
 |                                                                           |
 |                 Container Manager Dynamic Value Routines                  |
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
 CMSize CMReadValueData(CMValue value, CMPtr buffer, CMCount offset, CMSize maxSize);
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
 
 	1. The type, or any of its base types (if any, created by CMAddBaseType()), are for
 		 global names that have associated metahandlers registered by CMRegisterType().
		 
	2. The metahandlers support the operation type to return a "use value" handler, and
		 in addition for CMNewValue(), a "new value" handler.

 The "new value" handlers are used to define initialization data for the "use value"
 handlers.  The "use value" handlers are called to set up and return refCons.  Another
 metahandler address is also returned.  This is used to get the address of the value
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
 
 
 													The Data Structures for Dynamic Values
													--------------------------------------
 
 The following diagram shows the pointer relationships among the various data structures
 descending from an object for dynamic values.  Most of this is basically the same as
 shown in TOCEntries.[ch], so some of the superfluous pointers are removed to just show
 the pointers of interest.
 

 *----------*     *--------------------*               *---------------------*
 | *Object* |<--->|     *Property1*    |<------------->|     *Property2*     |
 *----------*  +->| (for these values) |           +-->|   (dynamic values)  |
               |  *--------------------*           |   *---------------------*
               |                                   |
               |                                   |
               +-->*-------------------*           +-->*--------------------*
                   |    *ValueHdr1*    |<--------------| *Dynamic ValueHdr* |
                   | (a "real" value)  |----+    +---->|   (bottom layer)   |
               +-->| (value not shown) |    |    |  +->|(on property chain) |
               |   *-------------------*    |    |  |  *--------------------*
               |                            |    |  |  |   *extensions*     |
               |                            |    |  |  |[DynamicValueVector]|
               |                            |    |  |  *--------------------*
               |                            |    |  |
               |                            |    |  +---------------*--------------------*
               |                            |    |                  | *Dynamic ValueHdr* |
               |                            |    |             +--->|   (middle layer)   |
               |                            |    |             |    *--------------------*
               |                            |    |             |    |   *extensions*     |
               |                            |    |             |    |[DynamicValueVector]|
               |                            |    |             |    *--------------------*
               |                            |    |             |
               |                            |    |             +---*---------------------*
               |                            +---------------------->| *Dynamic ValueHdr* |
               |                                 |                  |    (top layer)     |
               |                                 |                  *--------------------*
               |                                 |                  |   *extensions*     |
               |                                 |                  |[DynamicValueVector]|
               |                                 |                  *--------------------*
               |                                 |
               |                                 |
               |   *-------------------*         |     *--------------------*
               +-->|    *ValueHdr2*    |         +---->| *Dynamic ValueHdr* |
                   | (a "real" value)  |<------------->|   (single layer)   |
                   | (value not shown) |               |(on property chain) |
                   *-------------------*               *--------------------*
                                                       |   *extensions*     |
                                                       |[DynamicValueVector]|
                                                       *--------------------*


 As usual, for each object we have a list of properties, and each property has a list of
 value headers (the value segment lists are not shown).  Real values that spawn dynamic
 values cause a special property used only to contain the dynamic value headers.
 
 In the above diagram there are two "real" values that spawned dynamic values; "ValueHdr1"
 and "ValueHdr2".  "ValueHdr1" generated a three-layer dynamic value.  This results from
 the subclassing of types (subclassing is discussed later). "ValueHdr2" has only a single 
 layer dynamic value.

 The link structure is such that the "real" value always points to the top-most dynamic
 value layer.  That, in turn, points to its base value.  This continues until the bottom-
 most base value. It always points back to the "real" value.
 
 Note, the bottom-most layer (the one who has the "real" value as its base) is the ONLY
 value header on the dynamic property chain.  All higher layers for it are NOT part of the
 property chain.
 
 Note also that dynamic value headers never have value segment lists.  No data is ever 
 written to a dynamic value because these headers are removed when the value is released
 using a CMReleaseValue().  If there is any data, it must be associated with a "real"
 value -- the real value associated with a dynamic value or some place else (how this can
 happen is described later).
 
 In each value header there is a pointer (a union called "dynValueData" with alternative
 fields called "dynValue" and "extensions") that contains three possible values:
 
 		1. dynValueData is NULL for "real" value headers that don't have a corresponding
			 dynamic value.
		
		2. dynValueData.dynValue is a pointer to the top-most layer if it is a "real" value
			 that does have a corresponding dynamic value header.
			 
	  3. dynValueData.extensions is a pointer to the extensions if it is itself a dynamic
			 value header.
		
 The value header's flags determine how to interpret the pointer.
 
 The extensions is the interesting part.  It contains the vector of function pointers to
 the value handlers mentioned earlier (not unlike a C++ v-table).
 
 When a dynamic value is spawned by CMNewValue() or CMUseValue(), the pointer to the top-
 most dynamic value header is returned as the refNum.  That means whenever the user passes
 it to an API value routine, it will check to see if the refNum is a dynamic value.  If it
 is, it initiates the call to the corresponding value handler using the vector in the
 extensions.  That may cause a search up the base value chain to look for the "inherited"
 value routine.  In the limit, we end up using the original API value routine if no handler
 is supplied and we reach the "real" value in the chain.  That's how data could get in
 there.
		
 Assuming you're still with me, the following diagram is provided to simplify the
 notation.  We use this simplification of the diagram above so you can understand the
 meaning.  We will need this to notation to describe how dynamic values are spawned and
 layers created.  There is no need now to show all the details as above.


                  *---*     *---*      *-----*
                  | O |-----| P |------| DVP |
                  *---*     *---*      *-----*
                              |           |
                              |           |
                           *-----*     *-----*       *-----*       *-----*
                           | VH  |<----| DVH |<------| DVH |<------| DVH |
                           *-----*     *-----*       *-----*   +-->*-----*
                              |  |        |                    |
                              |  |        |                    | 
                              |  +-----------------------------+
                              |           |
                              |           |
                           *-----*     *-----*
                           | VH  |<--->| DVH |
                           *-----*     *-----*

 Here "O" is object, "P" is property, "VH" real value header, "DVP" the dynamic value
 property, and "DVH" a dynamic value.
 
 
                                  Spawning Dynamic Values
																	-----------------------
																	
 When a CMNewValue() or CMUseValue() is almost done, a check is made on the value's type,
 and all of its base types (if any) to see if it has an associated registered metahandler.
 If it does, it is called with a "use value" operation type to see if a "use value" handler
 exists for the type.  If it does, we spawn the dynamic value.
 
 To summarize, if CMNewValue() or CMUseValue() sees any (base) type that has an associated
 "use value" handler, it will spawn a dynamic value.
 
 The spawning is done, leaving out some confusing details, by calling the "use value"
 handler.  It is expected to set up a refCon to pass among the value handlers and a pointer
 to another metahandler.  These are returned to CMNewValue() or CMUseValue() which uses
 newDynamicValue() (defined in this file) to do the actual creation of the dynamic value.
 The extensions are initialized, the metahandler pointer saved, and also the refCon.  The
 pointer to the created dynamic value header is what CMNewValue() or CMUseValue()() returns
 to the user as the refNum.
 
 Now, when the user attempts to do a value operation using this refNum, a check is made
 that the refNum is for a dynamic value.  If it is, the corresponding handler routine will
 be called. The vector entries are set on first use of a value operation.  It may mean
 searching up the base value chain, but once found, we save the handler address in the top
 layer vector (associated with the refNum) so we don't have to do the search again.
 
 Note that if we indeed do have to search up the base value chain then we must save the 
 dynamic value refNum (pointer) in addition to the handler address.  This is very much like
 C++ classes, where inherited methods are called and the appropriate "this" must also be
 passed.  The "this" in this case is the refNum.
 
 
																Layering Dynamic Values
																-----------------------
 
 The best way to describe layering is in terms of C++.  Say we have the following class
 types (using a somewhat abbreviated notation -- don't get picky here):
 
  class Layer1 {																// a base class
 		<layer1 data>																// possible data (fields)
		Layer1(<layer1 args>);											// constructor to init the data
		other methods...														// value operations in our case
  };
 
  class Layer2 {																// another base class
 		<layer2 data>																// possible data (fields)
		Layer2(<layer2 args>);											// constructor to init the data
		other methods...														// value operations in our case
  };
	
	class T: Layer1, Layer2 {											// the class of interest!
		<T data>																		// possible data (fields)
		T(<T args>, <layer1 args>, <layer2 args>);	// constructor to init the data and bases
		other methods...														// value operations in our case
	};

 In Container Manager terminology, T is to be a registered type with other registered 
 types as base types (classes).  All type objects are created using the standard API call
 CMRegisterType().  Base types can be added to a type by using CMAddBaseType().  This
 defines a form of inheritance like the C++ classes.  
 
 Type T would be registered with its base types as follows:
 
 layer1 = CMRegisterType(container, "Layer1");
 layer2 = CMRegisterType(container, "Layer2");
 
 t = CMRegisterType(container, "T");
 CMAddBaseType(t, layer1);
 CMAddBaseType(t, layer2);
 
 Internally, the t object looks something like the following:
 
                  *---*     *-------*         *-------*
                  | t |-----| P[gn] |---------| P[bt] |
                  *---*     *-------*         *-------*
                                |                 |
                                |                 |
                             *-----*  *---*    *-----*  *--------*  *--------*
                             | VH  |--|"T"|    | VH  |--|"layer1"|--|"layer2"|
                             *-----*  *---*    *-----*  *--------*  *--------*
 
 The value data segments are shown here with the data the segment will point to in the
 container.
 
 For the t object, the global name property and value are created as usual by  
 CMRegisterType(container, "T").  The CMAddBaseType() calls add the base types.  These are
 recorded as the object ID's for each base type in the order created as separate value
 segments for a special "base type" property belonging to the type object.
 
 As mentioned above, CMNewValue() or CMUseValue() spawn dynamic values if the original type
 or any of its base types have an associated "use value" handler.  Assume that was done for
 "T" in the above example.  What happens is that CMNewValue() or CMUseValue() will look at
 its type object (t here) to see if the base type property is present.  If it is, it will
 follow each type "down" to leaf types using a depth-first search.
 
 In the example, "layer1" will be visited, then "layer2", and finally the original type
 "T" itself.  If the "layer1" type object had base types of its own, they would be visited
 before using "layer1" itself.  Hence the depth-first search down to the leaf types.
 
 For each type processed, if it has a "use value" handler of its own, it will be called
 to get a refCon and value handler metahandler.  These are passed to newDynamicValue()
 to create a dynamic value for the original "real" value.  newDynamicValue() always
 returns its refNum which will be the dynamic value it created.  The first layer will
 create the dynamic value property and put the dynamic value header on its value header
 list.  All further calls to newDynamicValue() will pass the most recent refNum returned
 from it.  newDynamicValue() then chains these off the first dynamic value header.  This
 produces the desired layering result.
 
 Note that this scheme allows total freedom for the user to mix types.  For example, type
 T1 could have base types T2 and T3.  Alternatively, T1 could just have base type T2 and
 T2 have T3 as its base type!
 

																 Data For Dynamic Values
																 -----------------------
 
 In the C++ class types shown above, note that each class could have its own data along
 with its own constructor.  The T class has a constructor that calls the constructors of
 all of its base classes.  We can carry this simile with the Container Manager just so far!
 Here's is where it starts to break down.
 
 The problem here is that C++ class types are declared statically.  A C++ compiler can see
 all the base classes and can tell what data gets inherited and who goes with what class.
 In the Container Manager, all "classes" (i.e., our type objects) are created dynamically!
 So the problem is we need some way to tell what data "belongs" to what type.
 
 The solution is yet another special handler, which returns a "format specification",
 called "metadata".  The handler is the "metadata" handler whose address is determined
 by the Container Manager from the same metahandler that returns the "new value" and "use
 value" handler addresses.
 
 Metadata is very similar to C printf() format descriptions, and as will be described
 shortly, for similar purposes.  The next section will describe the metadata in detail.
 But, for now, it is sufficient to know that it tells CMNewValue() how to interpret its
 "..." parameters.  The rest of this section will discuss how this is done to dynamically
 create data. 
 
 As with C++ classes, the data is created when a new value is created, i.e., with a
 CMNewValue() call.  The data will be saved in the container, so CMUseValue() uses the
 type format descriptions to extract the data for each dynamic value layer (actually,
 cmFollowTypes() in this file which is called by CMNewValue()).
 
 CMNewValue() is defined as follows:
 
 	  CMValue CMNewValue(CMObject object, CMProperty property, CMType type, ...);
 
 The "..." is an arbitrary number of parameters used to create the data.  Metadata,
 accessed from the "metadata" handler, tells CMNewValue() how to interpret these parameters
 just like a printf() format tells it how to use its arguments.

 The ORDER of the parameters is important!  Because base types are done with a depth-first
 search through the types down to their leaves, the CMNewValue() "..." parameters MUST be
 ordered for the "deepest" type first.  For example, given the following type inheritance
 hierarchy (read these as T1 inherits from T2 and T3, and so on):
 
                                      T4      T5
                                        *    *
                                         *  *
                                  T2      T3
                                    *    *
                                     *  *
                                      T1
							
 The depth-first search, starting at T1, yields the sequence: T2 T4 T5 T3 T1.  Then this
 is the order the CMNewValue() "..." parameters must be in.

 Note what's happening here is you are supplying all the constructor data just like T
 constructor class example above.
  
 The way the data gets written is with special handler, called the "new value" handler.
 After CMNewValue() calls the "metadata" handler, it uses the metadata to extract the next
 set of CMNewValue() "..." parameters.  CMNewValue() then passes the parameters along in
 the form of a data packet to the "new value" handler.  The "new value" handler is then
 expected to use this data (which it can extract with CMScanDataPacket()) to WRITE data
 to its BASE value.  It is the data written by the "new value" handler that the "use value"
 handler will read to create its refCon. 
 
 Only CMNewValue() does this.  The "new value" handler is only for new values, but the "use
 value" handler is used by both CMNewValue() and CMUseValue().
 
 In the simplest case, with only one dynamic value, you can see that the data is written 
 to the "real" value.  Now if you layer another dynamic value on to this, the next chunk
 of data is written using that layer's base value and hence through its handlers.  The
 second layer will thus use the first layers handlers.  That may or may not end up writing
 to the "real" value depending on the kind of layer it is.  If it's some sort of indirect
 handler (i.e., it reads and writes somewhere else), the second layer data will probably
 not go to the "real" value.
  
 The "use value" handler is called both for CMNewValue() and CMUseValue().  The "use value"
 handler reads the data from its base value to create its refCon.  If the user comes back
 the next day and does a CMUseValue(), only the "use value" handler is called.  Again
 it reads the data from its base value to construct the refCon and we're back as we were
 before in the CMNewValue() case.
 
 It should be pointed out here that the "matadata" and "new value" handlers will always
 be executed with a Container Manager running on some particular hardware (obviously).  The
 data packet built from the CMNewValue() "..." parameters is stored as a function of the
 hardware implementation on which it is run (i.e., whatever the sizes are for bytes, words,
 longs, etc.).  How it is stored is a function of the metadata returned from the "metadata"
 handler.  In other terms, the "new value" handler has a "contract" with both the Container
 Manager and the "metadata" handler on the meaning of the parameter data. 
 
 Note, however, it is NOT required that you be on the same hardware when you come back the
 next day and call CMUseValue() that leads to the "use value" handler call.  The handler
 writer must keep this in mind.  Specifically, the "use value" handler MUST know the
 attributes (bytes size, big/little endian, etc.) of the data written out by the
 "new value" handler so it knows how to use that info.  In other words, the "use value"
 handler has a (separate) "contract" with its own "new value" handler on the meaning of the
 data written to the base value.
 
 There is another, relatively minor, thing to keep in mind.  That is that the value
 handlers for any one layer must take into account the size of its own data when
 manipulating additional data created by its handlers for CMReadValueData(),
 CMWriteValueData(), etc.  This simply offsets the write and read value data operations
 by the proper amount.  Remember all operations are on their base values.  So if a "new
 value" handler writes data, this basically prefixes the "real" stuff being written by the
 handler operations.
 

 																 		Metadata
																 		--------

 As mentioned above, the metadata directs CMNewValue() on how to interpret its "..."
 parameters to build data packets passed to "new value" handlers.
 
 The format string is a sequence of characters containing data format specifications. 
 Unlike printf(), anything other than the data format specifications are ignored.  They
 are assumed to be comments.
 
 The data format specifications indicate to CMNewValue() how to interpret its data
 initialization parameters.  Each specification uses (consumes) the next corresponding
 "..." parameter to CMNewValue().  This is similar to the behavior of printf().
  
 A data format specification begins with a "%" sign.  Immediately following the % is a 
 required data size layout specification, expressed as a sequence of characters.  The data
 size specification sequences are as follows:
 
		c					A character or byte [note 1].
		
		d					A short [note 1].
		
		l[d]			A long (the "d" is optional) [note 1].
		
		[*]s			A C string (i.e., null delimited).  Optionally a "*" indicates that only the
							first n characters of the string are to be used.  The "*" consumes an
							additional CMNewValue() "..." parameter of type CMSize [notes 3, 4, and 5].	
							 
		i					An object, property, or type ID.  This is defined as the same size as "ld"
							[note 1].
		
		p					A pointer [notes 1 and 2].
	 
		o					A CMObject, CMProperty, or CMType object refNum.  This is defined as the
							same size as "p" [notes 1 and 2].
		
		v					A CMValue refNum.  This is defined as the same size as "p" [notes 1 and 2].

 Notes:
		
		1. The CMNewValue() "..." parameters are converted to a packet of data using the 
			 hardware implementation defined sizes for bytes, words, longs, etc., as directed by
			 the metadata returned from the "metadata" handler. Thus the "metadata" handler has
			 a contract with the "new value" handler that CMNewValue() calls.  The data that the
			 "new value" handler writes to its base value is in terms of a "contract" it has with
			 its "use value" handler.  It is the one that will read that base value data to
			 create its refCon.  If CMUseValue() is expected to be run on different hardware with 
			 different byte sizes, endianess, etc., then that is between the "new value" handler
			 and its "use value" handler.  The Container Manager is independent of that.
			 
		2. Pointers can be passed to CMNewValue() to convey special information to the "use
			 value" handler.  You shouldn't, of course, write these as data.  RefNums can be
			 passed to extract object ID's or other read value data.  It is permissible  to
			 write object ID's to data.  But this will put a restriction on such references that
			 they shouldn't be moved or deleted.
			
		3. For "%*s", the value corresponding to the "*" is copied to the packet data
			 immediately in front of the string.  This is somewhat (not quite) equivalent to
			 "%l%s", where the %l is the length, n, and %s is an n byte string.  Note however,
			 this string is NOT null delimited.
	 
		4. Caution, the string will be COPIED from the string pointed to in the CMNewValue()
			 "..." parameter list to the packet.  It you intend to pass a pointer to the string,
			 rather than the string itself, %p should be used.  Frankly, it is expected that %s
			 will not be used much!
		
		5. For symmetry, CMScanDataPacket() returns the value of string length to an explicit
			 distinct parameter pointer.  Thus the parameter pointer list passed to
			 CMScanDataPacket() should be identical to the "..." parameters passed to a
			 CMNewValue() "..." parameter list (at least the portion corresponding to this type).
			 

 								   The "Metadata", "New Value", and "Use Value" Handlers
								   -----------------------------------------------------
 
 The "metadata" handler is only needed for CMNewValue() so that the proper number of
 CMNewValue() "..." parameters can be placed into a data packet for the "new value"
 handler.
 
 The "metadata" handler should have the following prototype:
 
 CMMetaData metaData_Handler(CMType type);
 
 where, 
 
		type = the (base) type layer whose metadata is to be defined
 
 The "metadata" handler simply returns a C string containing the metadata using the format
 descriptions described above.
 
 The type is passed as a convenience.  It may or may not be needed.  It is possible for
 a type object to contain OTHER data for other properties.  Types, after all, are ordinary
 objects.  There is nothing prohibiting the creation of additional properties and their
 values.  This fact could be used to add additional (static and private) information to a
 type to be used elsewhere.
 
 Note, as in printf(), if the metadata handlers "lie" about the metadata format, or if 
 there aren't enough parameters supplied to CMNewValue(), the results will be
 unpredictable! 
 
 The "new value" handler should have the following prototype:

 CMBoolean newValue_Handler(CMValue baseValue, CMType type, CMDataPacket dataPacket);
 
 where, 
 	
		baseValue  = the base value which is to be used to write the refCon data for the
							 	 "use value" handler
		
		type			 = the type corresponding to this "new value" handler
		
		dataPacket = the pointer to the data packet, created from the CMNewValue() "..."
								 parameters according the types metadata format description

 The type is passed again as a convenience just as in the "metadata" handler.  It can also
 be used here to pass to CMScanDataPacket() to extract the dataPacket back into variables
 that exactly correspond to that portion of the CMNewValue() "..." parameters that
 correspond to the type.  It is not required, however that CMScanDataPacket() be used.
  
 The "use value" handler is called for both the CMUseValue() and CMNewValue() cases.  If
 it's companion "new value" handler wrote data to its base value, the "use value" handler
 will probably read the data to create its refCon.  The refCon will be passed to all
 value handlers.  The "use value" handler returns its refCon along with another
 metahandler address that is used to get the value operation handler addresses.  These are
 then used to create the dynamic value.
 
 The "use value" handler should have the following prototype:
 
 CMBoolean useValue_Handler(CMValue baseValue, CMType type, CMMetaHandler *metahandler, 
 											 			CMRefCon *refCon);
												
 where,
 		
		baseValue   = the base value which is to be used to write the refCon data for the
							 	  "use value" handler.
		
		type			  = the type corresponding to this "new value" handler

		metahandler = a pointer to the value operations metahandler which is RETURNED by the
									"use value" handler to its caller
									
		refCon			= a reference constant built by the "use value" handler and RETURNED to
								  its caller
 
 The baseValue and type are identical to the ones passed to the "new value" handler.  The
 type may or may not be needed in the "use value" handler.  Like the "use value" handler,
 it could be used to supply additional information from other type object properties.
 
 It is expected that the "use value" handler will read data from its base value to
 construct its refCon.  The refCon is then returned along with a pointer to another
 metahandler that is used by the Container Manager to get the addresses of the value
 operation routines.
  
 Note, both the "new value" and "use value" handlers return a CMBoolean to indicate 
 success or failure.  Failure means (or it is assumed) that the handlers reported some
 kind of error condition or failure.  As documented, error reporters are not supposed to
 return.  But in case they do, we use the CMBoolean to know what happened.   It should 
 return 0 to indicate failure and nonzero for success.


															Value Operation Handlers
															------------------------
															
 The value operation routines can do a CMGetValueRefCon() on the value passed to get at the
 refCon set up by the "use value" handler.  This provides a communication path among the
 value handlers.  Further, the value handler should usually do their operations in terms of
 their base value.  This is accessed using CMGetBaseValue() on the passed value.
 
 There is one exception to this rule; the release handler.  A set of one or more dynamic
 value layers are spawned as a result of a single CMUseValue() or CMNewValue().  The layers
 result from the specified type having base types.  From the caller's point of view s/he
 is doing one CMUseValue() or CMNewValue() with no consideration of the base types.  That
 implies that the returned dynamic value should have a single CMRelaseValue() done on it.
 The handlers have no business doing CMReleaseValue() on their base value.  This is
 detected and treated as an error.
 
 A count is kept by the Container Manager of every CMUseValue() and CMNewValue().  Calling 
 CMReleaseValue() reduces this count by one.  When the last release is done on the dynamic
 value (its count goes to 0), the release handler will be called.  It is the Container
 Manager who calls the release handler for all the layers, not the handler.  The Container
 Manager created them as a result of the original type.   It is therefore responsible for
 releasing them.
 
 The reason the Container Manager is so insistent on forcing a release for every use of a
 dynamic value is mainly to enforce cleanup.  Most value operation handlers will, at a
 minimum, use a refCon that was memory allocated by the "use value" handler.  Release
 handlers would then be responsible for freeing that memory.  In another example, if any
 files were open by the "use value" handler, the releases would close those files.
 
 Note, if all a value operation does is get its base value and call back the API routine
 to do its operation (again except for the release handler), then what it is basically
 doing is invoking the "inherited" method ('er, sorry, value operation).  In this case,
 the value operation could be OMITTED entirely.  That is, don't define a routine address
 in the value operations metahandler (returned by the "use value" handler).  The Container
 Manager uses that as the signal to search up the dynamic value inheritance chain to find
 the first handler defined for the routine.  In the limit, it will end up using the
 original "real" value.
 
 
 							Possible Limitations and Considerations On Value Operations
							-----------------------------------------------------------
							 
 As presented, the model for type inheritance described above only allows for inheriting
 value operations that would otherwise be processed via the API.  A non-obvious fact of
 this is that value operations are basically limited to stream operations.  That is,
 you read or write a chunk of stuff linearly from a specified offset.  In addition, there
 are insert and delete operations (CMInsertValueData() and CMDeleteValueData()).  These two
 cause potential problems!
 
 The reason they are a problem is that base types may want to do certain transformations
 on their data that depend on what has occurred previously in that stream of data.  For
 example, encryption, using a cyclic key, or compression, which can be thought of as a form
 of encryption, generally cannot be done simply by looking at a chunk of data starting at
 some random offset.  A cyclic key encryption can be deterministic if you can always
 determine where to start in the key as a function of offset. But you can see that inserts
 and deletes will change the offsets of following data.  You would not know where to start
 in the key.
 
 What all this means is that certain data transformations only make sense if you are
 willing to refuse to support the insert/delete operations.  Basically only data
 transformations that are position independent can be fully supported.
 
 Even simple I/O to a file may create problems, since most file systems do not support
 inserts and deletes in the middle of a file.  If you do want to support inserts and
 deletes, then you should consider the potential for data intensive and/or computationally
 intensive operations.
 
 Consider yourself warned!

 Well, that's all there is to dynamic values.  Aren't they fun?  I am sorry about this
 long winded explanation, but there is no short way to describe this stuff.  Would you
 rather I didn't?
*/


#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __CM_API__
#include "CMAPI.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __DYNAMICVALUES__
#include "DynValus.h"     
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __ERRORRPT__
#include "ErrorRpt.h"      
#endif
#ifndef __UTILITYROUTINES__
#include "Utility.h"        
#endif

																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment CMValueOps
#endif

/* Note, this is the same segment used by  CMValOps.c  intentionally.										*/


/* For generality (or whatever), the following define all the fields of a metadata 			*/
/* format description...																																*/
	
#define Meta_Trigger 		'%'						/* all formats are triggered by this							*/
#define Meta_Ptr 				'p'						/* pointers																				*/
#define Meta_Object 		'o'						/* CMObject, CMProperty, or CMType object refNum	*/
#define Meta_Value 			'v'						/* CMValue refNum																	*/
#define Meta_ID 				'i'						/* object, property, or type ID										*/
#define Meta_Char 			'c'						/* character or byte															*/
#define Meta_Short 			'd'						/* short																					*/
#define Meta_Long 			'l'						/* long ("%ld" also allowed)											*/
#define Meta_LongSuffix 'd'						/* optional "long" suffix													*/
#define Meta_Star 			'*'						/* string of n characters (uses as "*s")					*/
#define Meta_String 		's'						/* C string																				*/


/*-------------------------------------------------------------------------------*
 | hasUseValueHandler - check a base type to see if it has a "use value" handler |
 *-------------------------------------------------------------------------------*
 
 This routine takes a value (header) checks to see if the value type is a global name.  If
 it is (what else?), and if there is a metahandler associated with the name (as defined by
 an earlier CMSetMetaHandler() call), then that metahandler is called with a "use value"
 operation type.  If the methandler returns the address of a "use value" handler, the
 function returns that address as its result.  If newValueHandler is not NULL then the
 "new value" handler address AND a "metadata" handler address are also required and
 returned.  NULL is returned if the metahandler or "use value" handler do not exist.
 
 Note, the "new value" and "metadata" handlers are requested only for CMNewValue() creation
 of dynamic values.  That is the only time newValueHandler is specified.  Thus an error
 will be  reported from here, and SessionSuccess (a session global status switch) returned
 false, if the "new value" or "metadata" handler is not present when requested.  Otherwise
 SessionSuccess is always retured as true.
*/

static CMHandlerAddr CM_NEAR hasUseValueHandler(TOCValueHdrPtr baseValueHdr,
																								TOCObjectPtr type, 
																								CMHandlerAddr *newValueHandler,
																								CMHandlerAddr *metaDataHandler)
{
	ContainerPtr 	 container = baseValueHdr->container;
	CM_CHAR	 			 *typeName;
	MetaHandlerPtr metaHandler;
	CMHandlerAddr  useValueHandler;
	
	SessionSuccess = true;											/* assume success													*/
	
	/* Check to see if the type object is for a global type name...												*/
	
	if ((typeName = cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName)) == NULL) 
		return (NULL);														/* no dynamic value for this type					*/
		
	/* Check to see if the type name has an associated metahandler...											*/
		
	metaHandler = cmLookupMetaHandler((CM_UCHAR *)typeName, container->sessionData);
	if (!SessionSuccess) {											/* if allocation error...									*/
		ERROR1(CM_err_HandlerError, typeName);		/* ...yell																*/
		metaHandler = NULL;
	}
	
	/* If there is a metahandler, check to see if the metahandler has a "use value" 			*/
	/* handler...																																					*/

	if (metaHandler == NULL || metaHandler->metaHandler == NULL)
		return (NULL);
		
	useValueHandler = (CMHandlerAddr)(*metaHandler->metaHandler)(NULL, (CMGlobalName)CMUseValueOpType);

	if (useValueHandler == NULL) 								/* no "use value" handler ==> not dynamic	*/
		return (NULL);
	
	/* If the caller requested the "use value" handler, and we didn't find one, we assume */
	/* the type is NOT for a dynamic value.  If we have the "use value" handler and the		*/
	/* new value handler is not needed, simply return the "use value" handler as the 			*/
	/* function result.  However, if the "new value" handler is requested, and it's not 	*/
	/* there, we have an error.  Note that if the "new value" handler is needed, then the	*/
	/* "metadata" handler will be also.  It's both or neither.  The "metadata" handler is	*/
	/* always required just the "new value" handler for CMNewValue().											*/
	
	if (newValueHandler == NULL) 								/* if "new value" handler not needed...		*/
		return (useValueHandler);									/* ...return "use value" handler					*/
	
	*newValueHandler = (CMHandlerAddr)(*metaHandler->metaHandler)(NULL, (CMGlobalName)CMNewValueOpType);
	if (*newValueHandler == NULL) { 						/* return "new value" handler if we got it*/
		ERROR1(CM_err_NoNewValueHandler,typeName);/* ...yell again													*/
		SessionSuccess = false;
		return (NULL);
	}
	
	*metaDataHandler = (CMHandlerAddr)(*metaHandler->metaHandler)(NULL, (CMGlobalName)CMDefineMetaDataOpType);
	if (*metaDataHandler == NULL) { 						/* return "metadata" handler if we got it	*/
		ERROR1(CM_err_NoMetaDataHandler,typeName);/* ...yell again and again								*/
		SessionSuccess = false;
		return (NULL);
	}
	
	return (useValueHandler);										/* tell cmFollowTypes() it's dynamic 			*/
}


/*---------------------------------------------------------------------------------------*
 | createDataPacket - allocate data packet for "new value" handler using type's metadata |
 *---------------------------------------------------------------------------------------*
 
 For a given type, its "metadata" (determined by the type's "metadata" handler) is used to
 consume constructorData parameters (a CMNewValue() "..." parameter list).  The parameters
 are used to construct a data packet which will be passed to a "new value" handler.
 
 The packet is allocated using the memory allocator handler.  The caller (cmFollowTypes())
 is expected to free the allocation.  The pointer to the packet is returned as the
 function result.  The constructorData is updated to point at the first "..." parameter
 not consumed (if any).
 
 Note, a null byte is added to the end of the data packet to make life a little easier for
 cmVScanDataPacket() if that is used to extract the data into variables.
 
 NULL is returned if no packet is allocated because there is no metadata.
 
 SessionSuccess, a session global status switch, is returned false if an allocation error
 or inconsistency data check is reported.  It is returned true otherwise.
*/

static CM_UCHAR *CM_NEAR createDataPacket(TOCObjectPtr type, CM_CHAR *metaData, va_list *constructorData)
{
	ContainerPtr 	 container = type->container;
	CM_UCHAR			 c, *s, *dataPacket, *dp;
	CM_CHAR				 *mp, m, *typeName, insert[10];
	CM_ULONG			 packetSize, l;
	CM_USHORT			 d;
	CMSize				 n;
	void					 *p;
	va_list				 ap;
	
	SessionSuccess = true;									/* assume success															*/
	
	/* If there is no metadata, return NULL to indicate there's no data packet...					*/
	
	if (metaData == NULL || *metaData == '\0') return (NULL);
	
	/* Prescan the metaData to see how large it is.  We must keep in sync with the 				*/
	/* CMNewValue() "..." parameters because some of the metadata refer may refer to 			*/
	/* strings (%s and %*s).  Thus the size is not a simple function of metadata length.	*/
	
	packetSize = 0;													/* sum packet size in here										*/
	mp = metaData;													/* mp will be metaData pointer								*/
	ap = *constructorData;									/* leave original constructorData ptr alone		*/
	
	while ((CMBoolean)(m = *mp++)) {					/* scan the metaData...												*/
		if (m != Meta_Trigger) continue;			/* if not format specification, loop					*/
		
		switch ((CMBoolean)(m = *mp++)) {
			case Meta_Value:																				/* %v											*/
			case Meta_Object:																				/* %o											*/
			case Meta_Ptr:		p = va_arg(ap, void *);								/* %p											*/
												packetSize += sizeof(void *);
												continue;
			
			case Meta_Char:		c = va_arg(ap, CM_UCHAR);							/* %c											*/
												packetSize += sizeof(CM_UCHAR);
												continue;
			
			case Meta_Short:	d = va_arg(ap, CM_USHORT);						/* %d											*/
												packetSize += sizeof(CM_USHORT);
												continue;
			
			case Meta_Long: 	if (*mp == Meta_LongSuffix) ++mp;			/* %l[d]									*/
			case Meta_ID:			l = va_arg(ap, CM_ULONG);				/* %i											*/
												packetSize += sizeof(CM_ULONG);
												continue;
			
			case Meta_Star:		if ((m = *mp++) != Meta_String) {			/* %*s										*/
													insert[0] = '*';
													insert[1] = m;
													insert[2] = '\0';
													typeName = cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName);
													ERROR3(CM_err_BadMetaSpec, insert, typeName, CONTAINERNAME);
													return (NULL);
												}
												n = va_arg(ap, CMSize);
												s = va_arg(ap, CM_UCHAR *);
												packetSize += n + sizeof(CMSize);
												continue;
												
			case Meta_String:	s = va_arg(ap, CM_UCHAR *);						/* %s											*/
												packetSize += strlen((CM_CHAR *)s) + 1;
												continue;
			
			default:					insert[0] = m;												/* oops!									*/
												insert[1] = '\0';
												typeName = cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName);
												ERROR3(CM_err_BadMetaSpec, insert, typeName, CONTAINERNAME);
												SessionSuccess = false;
												return (NULL);
		} /* switch */
	} /* while */
	
	/* Now that we got the size, and we're happy with the metaData, allocate the packet...*/
	
	dataPacket = (CM_UCHAR *)CMmalloc(packetSize + 1);	/* we add a null to the end		*/
	if (dataPacket == NULL) {
		typeName = cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName);
		ERROR2(CM_err_NoDataPacket, typeName, CONTAINERNAME);
		SessionSuccess = false;
		return (NULL);
	}
	
	/* Rescan the metadata and put the data into the newly allocated data packet.  Note,	*/
	/* data alignment is not assumed here.  We use memmove() to build the packet.					*/
	
	mp = metaData;													/* mp will be metaData pointer again					*/
	dp = dataPacket;												/* enter each data item at dp									*/
	ap = *constructorData;									/* set to consume parameters again						*/
	
	while ((CMBoolean)(m = *mp++)) {					/* rescan the metaData...											*/
		if (m != Meta_Trigger) continue;			/* if not format specification, loop					*/
		
		switch ((CMBoolean)(m = *mp++)) {
			case Meta_Value:																				/* %v											*/
			case Meta_Object:																				/* %o											*/
			case Meta_Ptr:		p = va_arg(ap, void *);								/* %p											*/
												memmove(dp, &p, sizeof(void *));
												dp += sizeof(void *);
												continue;
			
			case Meta_Char:		c = va_arg(ap, CM_UCHAR);							/* %c											*/
												memmove(dp, &c, sizeof(CM_UCHAR));
												dp += sizeof(CM_UCHAR);
												continue;
			
			case Meta_Short:	d = va_arg(ap, CM_USHORT);						/* %d											*/
												memmove(dp, &d, sizeof(CM_USHORT));
												dp += sizeof(CM_USHORT);
												continue;
			
			case Meta_Long: 	if (*mp == Meta_LongSuffix) ++mp;			/* %l[d]									*/
			case Meta_ID:			l = va_arg(ap, CM_ULONG);				/* %i											*/
												memmove(dp, &l, sizeof(CM_ULONG));
												dp += sizeof(CM_ULONG);
												continue;
			
			case Meta_Star:		++mp;																	/* %*s										*/
												n = va_arg(ap, int);									
												s = va_arg(ap, CM_UCHAR *);
												memmove(dp, &n, sizeof(CMSize));			/* (prefix with "*")			*/
												dp += sizeof(CMSize);								
												memmove(dp, s, (size_t)n);
												dp += n;
												continue;
												
			case Meta_String:	s = va_arg(ap, CM_UCHAR *);			/* %s											*/
												strcpy((CM_CHAR *)dp, (CM_CHAR *)s);
												dp += strlen((CM_CHAR *)s) + 1;
												continue;
		} /* switch */
	} /* while */
	
	*dp = (CM_UCHAR)0;											/* add null byte for cmVScanDataPacket()			*/
	
	*constructorData = ap;									/* point constructorData at next set of args	*/
	
	return (dataPacket);										/* return data packet to the caller						*/
}


/*---------------------------------------------------------*
 | newDynamicValue - create a new "dynamic" value (header) |
 *---------------------------------------------------------*
 
 This routine is called only by cmFollowTypes() to do the actual construction of the
 dynamic value for the specified base value.  The type is the type that is causing this
 dynamic value to be created.  If the base value is a "real" value, then the dynamic value
 is added to a "dynamic values" property for the object who owns the value.  The "dynamic
 value"s property is created for the first dynamic value for that object.  All further
 dynamic values with "real" value bases are simply added as values (headers) to that
 property.  If the base value is itself a dynamic value, then the newly created dynamic
 value is chained to the base (dynamic) value.  It is only a backward link from the new
 dynamic value.
 
 The pointer to the new dynamic value is returned.  NULL is returned if any errors are
 reported.  Note, the newOrUseValueName parameter is the string "CMNewValue" or
 "CMUseValue" and only used for some of the error inserts.
 
 By passing the returned dynamic value pointer as the base value to succeeding 
 newDynamicValue() calls, layers of dynamic values are constructed.
 
 See the dynamic value documentation at the start of this file for details on the data
 structures.
*/

static TOCValueHdrPtr CM_NEAR newDynamicValue(TOCValueHdrPtr baseValueHdr,
																							TOCObjectPtr type,
																							CMMetaHandler metaHandler,
													 		 				 				CMRefCon refCon,
																							CM_CHAR *newOrUseValueName)
{
	ContainerPtr 	 	  container = baseValueHdr->container;
	TOCObjectPtr	 	  theObject;
	TOCValueHdrPtr 	  theDynamicValueHdr, theRealValueHdr;
	DynValueHdrExtPtr extensions;
	CMBoolean					firstDynamicValue;
	CM_CHAR 					*typeName;
	
	if (metaHandler == NULL) {												/* we must have a metahandler!			*/
		typeName = cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName);
		ERROR2(CM_err_NoMetahandler, typeName, CONTAINERNAME);
		return (NULL);
	}
	
	/* It is an error for layered dynamic values to pass as the base value any other 			*/
	/* value header other than the most recent.  The code near the end of this routine 		*/
	/* chains layered dynamic values together with each value pointing back to its base. 	*/
	/* The original "real" value gets a pointer to the last one (in the dynValue union 		*/
	/* field of the value header). So by searching back up the chain to the "real" value 	*/
	/* we can get at this pointer to see if the base value passed is equal to it and 			*/
	/* report an error if it is not. Of course, the first time the "real" value is passed */
	/* and the dynValue will be NULL.  So we will know this case too.											*/
	
	theRealValueHdr = baseValueHdr;										/* start with the current base			*/
	while (IsDynamicValue(theRealValueHdr)) 					/* loop back till we find 1st value	*/
		theRealValueHdr = DYNEXTENSIONS(theRealValueHdr)->baseValue;/*that not dynamic value*/
	
	firstDynamicValue = (CMBoolean)(theRealValueHdr->dynValueData.dynValue == NULL);
	
	if (!firstDynamicValue &&													/* do the base check...							*/
			theRealValueHdr->dynValueData.dynValue != baseValueHdr) {
		ERROR2(CM_err_BadRealValue, newOrUseValueName, CONTAINERNAME);		/* ...oops!				*/
		return (NULL);
	}
	
	/* Create the dynamic value, i.e., a value (header) that is belongs to a special 			*/
	/* property of the caller's object that owns the base value.  The object "owning" the	*/
	/* dynamic value is flagged to indicate there are dynamic values present in that 			*/
	/* object.  This protects it from premature deletion.  The flag is cleared when the 	*/
	/* last dynamic value for for the object is released.  We don't use the "protected"		*/
	/* flag, which is used for similar purposes, just to be general.  Maybe we could 			*/
	/* someday have a protected object with dynamic values! 															*/
	
	/* Note, the type we give the dynamic value is that of the type that caused this 			*/
	/* dynamic value to be generated.  We never use that fact.  But we got to give it 		*/
	/* something and it could aid in debugging this turkey! Do you really think I am 			*/
	/* going to get all this right the first time?  Want to buy a bridge?									*/
		
	theObject = cmDefineObject(container, baseValueHdr->theProperty->theObject->objectID,
														 CM_StdObjID_DynamicValues,
														 0, /* 0 stops dup checks, was "type->objectID"							*/
														 NULL, container->generation, kCMDynValue, 
														 ObjectObject, &theDynamicValueHdr);
	if (theObject == NULL) return (NULL);
	theDynamicValueHdr->typeID = type->objectID;		/* give the type ID something valid		*/
	theObject->objectFlags |= DynamicValuesObject;	/* make sure object is flagged				*/
	
	/* The dynValueData field in the value header is a union with two alternatives.  The	*/
	/* "dynValue" alternative is a pointer from the base value header to its dynamic			*/
	/* value.  The "extensions" alternative is for the dynamic value header itself and		*/
	/* points to the additional information, i.e., extensions, needed to accompany a 			*/
	/* dynamic value header.  A cmDefineObject() call always initializes the union field	*/
	/* to NULL on the assumption a "normal" value header is being created.  Only in here	*/
	/* do we create dynamic value headers.  Thus only in here do we fill in these fields. */
	/* We just got done creating the dynamic value header.  So the first thing to do is 	*/
	/* to allocate its extensions data.  So here goes...																	*/
		
	if ((extensions = (DynValueHdrExtPtr)CMmalloc(sizeof(DynValueHdrExt))) == NULL) {
		typeName = cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName);
		ERROR2(CM_err_NoDynExtensions, typeName, CONTAINERNAME);
		return (NULL);
	}
	
	/* Point the extensions union alternative at the extensions space we just alloacted.	*/
	/* Also, while we're screwing around in the dynamic value save the refCon passed in.	*/
	/* The valueRefCon field is convenient so that CMGetValueRefCon() will be able to be	*/
	/* used by the dynamic value handlers just as it would with normal values.						*/
	
	DYNEXTENSIONS(theDynamicValueHdr) = extensions;		/* point at the extensions data			*/
	theDynamicValueHdr->valueRefCon 	= refCon;				/* remember the refCon							*/
	theDynamicValueHdr->useCount    	= 1;						/* set use count to 1st use					*/
	
	/* We now must init the extensions fields.  Among the fields are the dynamic value		*/
	/* handler pointer vector (our "v-table").  It consists of the pointers to the				*/
	/* corresponding value operation handler, a "protect-from-recursive-use" flag, and		*/
	/* the value "this" pointer.  We init all this stuff to NULL and "not called".  The		*/
	/* metahandler passed to this routine will be used to fill in the handler pointers on	*/
	/* first use (see cmGetDynHandlerAddress()).  It will set the "this" pointer too.			*/
	/* To make it easier to init in the vector, the following macro is used:							*/
	
	#define InitDynVector(h) extensions->dynValueVector.h.handler 	= NULL;  \
													 extensions->dynValueVector.h.thisValue = NULL;	 \
													 extensions->dynValueVector.h.active 	  = false;
	
	InitDynVector(cmGetValueSize);										/* prepare handler vector...				*/
	InitDynVector(cmReadValueData);
	InitDynVector(cmWriteValueData);
	InitDynVector(cmInsertValueData);
	InitDynVector(cmDeleteValueData);
	InitDynVector(cmGetValueInfo);
	InitDynVector(cmSetValueType);
	InitDynVector(cmSetValueGen);
	InitDynVector(cmReleaseValue);
	
	extensions->metaHandler = metaHandler;						/* remember metahandler address			*/
	extensions->baseValue	  = baseValueHdr;						/* point dyn value hdr to its base	*/
	
	/* For layering dynamic values, a backward chain of dynamic value headers is formed 	*/
	/* from the new dynamic value header back to the original "real" value header.  The  	*/
	/* link is the baseValue extensions field set above. It always points to its base 		*/
	/* value (it is also used by CMGetBaseValue()).  As mentioned above, the dynValue 		*/
	/* union alternative in a "real" value header always points to the last dynamic 			*/
	/* value of the chain.  A CMUseValue() or CMNewValue() will always return the pointer */
	/* to the dynamic value at the end of the chain.  So, we always want to set the 			*/
	/* dynValue pointer to the new dynamic value header we created above.  Note also we		*/
	/* use this pointer to make sure the value passed as a base to us here is indeed the	*/
	/* pointer to the end of the chain.  We did this check near the start of this routine.*/
	/* From that check we get the pointer to the original "real" value.  So now we can 		*/
	/* set that pointer to the new end of chain.																					*/
	
	theRealValueHdr->dynValueData.dynValue = theDynamicValueHdr;
	
	/* Layered dynamic values, i.e., dynamic values that have a dynamic value base are 		*/
	/* NOT treated as distinct values for special property whose chain we put them on when*/
	/* we did the cmDefineObject() above.  This means we must take such a value OFF the		*/
	/* special property chain.  It's not getting lost though.  Remember we just got done 	*/
	/* pointing the "real" value dynValue at it.  Note, we flag this value header as 		  */
	/* being off the chain.  This makes it a simple test later when we want to delete 		*/
	/* these puppies off the layering chain (done by CMReleaseValue()).										*/
	
	if (!firstDynamicValue) {													/* do only if not the first time		*/
		cmDeleteListCell(&theDynamicValueHdr->theProperty->valueHdrList, theDynamicValueHdr);
		cmNullListLinks(theDynamicValueHdr);
		theDynamicValueHdr->valueFlags |= ValueOffPropChain; /* mark as off the prop. chain	*/
	} else																						/* if we have a NEW dynamic value...*/
		++container->targetContainer->nbrOfDynValues;		/* ...keep count in targetContainer	*/
	
	return (theDynamicValueHdr);											/* give caller the dynamic value		*/
}


/*----------------------------------------------------------------------------------*
 | cmDeleteAllDynamicValueLayers - free memory for a single dynamic value layer set |
 *----------------------------------------------------------------------------------*
 
 This routine is used in two contexts; first, for error recovery to free all the memory
 for a newly allocated dynamic value layer set, and second, when freeing the entire TOC
 to make usre all memory for any "dangling" dynamic values are freed.
 
 The current top-most dynamic value layer is passed in theValueHdr.  The layers are
 back-linked up to the base "real" value.  All the dynamic value memory on this chain are
 freed.  If freeValueHdr is true, then we were creating dynamic values for CMNewValue().
 In that case, the real value itself is freed.  Really freed!  Not marked free as is the
 usual case.
 
 The cmMarkValueDeleted() is called to free the real value.  Its parameters are such that
 the value header memory will be freed rather than being put on its deleted values list.
 That routine will also delete the property if the value is the only one for its property.
*/

void cmDeleteAllDynamicValueLayers(TOCValueHdrPtr theValueHdr, CMBoolean freeValueHdr)
{
	TOCValueHdrPtr nextLayer;
	TOCPropertyPtr theProperty;
	ContainerPtr 	 container = theValueHdr->container;

	if (IsDynamicValue(theValueHdr)) {
		theProperty = theValueHdr->theProperty;						/* clear obj flag if last dyn valu*/
		if (cmCountListCells(&theProperty->valueHdrList) == 1)
			theProperty->theObject->objectFlags &= ~DynamicValuesObject;
		
		if (container->targetContainer->nbrOfDynValues>0) /* adjust total dyn value count		*/
			--container->targetContainer->nbrOfDynValues;			

		while (IsDynamicValue(theValueHdr)) {							/* free all these dyn values...		*/
			nextLayer=DYNEXTENSIONS(theValueHdr)->baseValue;/* ...remember the next layer up	*/
				
			if ((theValueHdr->valueFlags & ValueOffPropChain) == 0) /* ...del. from property	*/
				cmDeleteListCell(&theProperty->valueHdrList, theValueHdr); /*(bottom layer only)*/
		
			CMfree(DYNEXTENSIONS(theValueHdr));							/* ...free the extensions...			*/
			CMfree(theValueHdr);														/* ...and the header itself				*/
			
			theValueHdr = nextLayer;												/*	loop up to the "real" value		*/
		} /* while */
	}
	
	if (freeValueHdr)																		/* if CMNewValue() or freeing TOC	*/
		cmMarkValueDeleted(theValueHdr->container, theValueHdr, true);/* free new real value*/
}


/*-------------------------------------------------------------*
 | cmFollowTypes - follow a type through all of its base types |
 *-------------------------------------------------------------*
 
 This routine creates a dynamic value layers for the passed type and all of its base types,
 if any of these types have a "use value" handler.  This routine is only called by
 CMNewValue() or CMUseValue().  For CMNewValue(), "metadata" and "new value"  handlers are
 also required.  The top-most dynamic value header pointer is returned.  This should be
 returned from CMUseValue() or CMNewValue().  NULL is returned if an error is reported.
 The original "real" value is returned if no dynamic values are created.
	
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
 
 See dynamic value documentation at the start of this file for a description of the
 prototypes for the "metadata", "new value", and "use value" handlers, as well as the
 description of the metadata itself.
 
 To produce all the required dynamic values, cmFollowTypes() recursively follows the types,
 looking for base types as defined by CMAddBaseType().  Each type can have any number of
 base types.  The recursion effectively produces a depth-first search of all the base
 types.
 
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
 is the order the CMNewValue() "..." parameters must be in.  It is also the order the
 dynamic value layers are generated.  T1 is the top layer, and T2 the bottom.  T1 would be
 the dynamic value returned from CMUseValue() or CMNewValue().
*/

TOCValueHdrPtr cmFollowTypes(TOCValueHdrPtr baseValueHdr, TOCObjectPtr type,
														 CMBoolean isNewValue, va_list *constructorData)
{
	ContainerPtr 	 container = baseValueHdr->container;
	TOCObjectPtr	 baseType;
	TOCPropertyPtr baseTypeProperty;
	TOCValueHdrPtr theValueHdr, baseValueHdr0;
	TOCValuePtr 	 theValue;
	CMHandlerAddr	 useValueHandler, newValueHandler, metaDataHandler; 
	CM_UCHAR			 *dataPacket;
	CM_CHAR				 *newOrUseValueName, *metaData, *typeName;
	CMRefCon			 refCon;
	CMMetaHandler  metaHandler;
	CMBoolean			 success;
	
	/* If any errors are reported, and the error handler returns, we NULL out the base		*/
	/* value.  That essentialy "puts the breaks" on the dynamic value generation.  The 		*/
	/* NULL result will work ist way back to CMNewValue() or CMUseValue() which it will		*/
	/* return. 																																						*/
	
	/* Note, as soon as we detect an error in here, we free all the dynamic values we  		*/
	/* created for the "real" value.  That should put things back the way they were 			*/
	/* originally.  Of course, error reporters are not suppose to return.  We're being		*/
	/* "kind" here just in case they do.																									*/
	
	if (baseValueHdr == NULL) return (NULL);
	
	baseValueHdr0 = baseValueHdr;								/* save current baseValueHdr for errors		*/
	
	/* If the current type has any base types, call cmFollowTypes() recursively for each	*/
	/* of those base types.  That brings us back to here where we will check each of 			*/
	/* those base types for base types of their own.  This process continues down to the	*/
	/* "bottom", i.e., leaf types, where we bypass this piece of code. The types are then */
	/* processed as the recursion unwinds.  It is this code, then the effects the depth-	*/
	/* first processing of the base types. 																								*/
	
	baseTypeProperty = cmGetObjectProperty(type, CM_StdObjID_BaseTypes);
	
	if (baseTypeProperty != NULL) {							/* process this type's base types					*/
		theValueHdr = (TOCValueHdrPtr)cmGetListHead(&baseTypeProperty->valueHdrList);
		
		if (theValueHdr != NULL) {								/* double check for value hdr							*/
			theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList); /* 1st base ID		*/
			
			while (theValue != NULL) {
				baseType = cmFindObject(container->toc, theValue->value.imm.ulongValue);
				if (baseType) {
					baseValueHdr = cmFollowTypes(baseValueHdr, baseType, isNewValue, constructorData);
					if (baseValueHdr == NULL) return (NULL);
				}
			
				theValue = (TOCValuePtr)cmGetNextListCell(theValue); /* look at next base type	*/
			} /* while */
		
		} /* theValueHdr */
	} /* baseTypeProperty */
	
	/* We are now at a type which has either has no base types or that had all of its base*/
	/* types processed (again through here).  This is where we determine whether the type	*/
	/* will spawn a dynamic value.  We call hasUseValueHandler() that checks for the			*/
	/* the "use value" handler for the type.  It also checks for the "metadata" and "new 	*/
	/* value" handlers if we were originally called from CMNewValue().  If it doesn't pass*/
	/* the tests we return the original base value unchanged.  If it does pass the tests, */
	/* we create a new dynamic value (layer). 																						*/
	
	/* As descibed in the documentation at front of this file, we call the "metadata" and	*/
	/* "new value" handlers first only for CMNewValue(). For CMNewValue() or CMUseValue() */
	/* we then call the "use value handler.  For the "new value" call we must first build */
	/* the data packet from the CMNewValue() "..." parameters using the metadata returned	*/
	/* from the "metadata" handler.  The packet is sent to the "new value" handler. There */
	/* it can use CMScanDataPacket() to extract the packet back into variables.						*/
	
	/* Note, as the packets for the layers are created, the constructorData parameter			*/
	/* pointer marches across the CMNewValue() "..." parameters.  Since we are building		*/
	/* the layers as we move up the inheritance hierarchy, from bottom to top, that is 		*/
	/* what determines the parameter specifications discussed above.  Further, it is also	*/
	/* why the constructorData is a va_list*.  The higher layers must know what the				*/
	/* current parameter pointer is.  If we had built the hierarchy from top to bottom we	*/
	/* wouldn't have needed the "*".																											*/
	
	/* And speaking of layers -- the newDynamicValue() call is the one responsible for		*/
	/* creating a layer.  It builds upon the current baseValueHdr and returns a new one.	*/
	/* they are passed up the recursive calls as the function result to build the layer 	*/
	/* chain.  The final, top-most layer, is the one eventually returned to CMNewValue()	*/
	/* or CMUseValue(), who then returns it to the user.																	*/
	
	useValueHandler = hasUseValueHandler(baseValueHdr, type, isNewValue ? &newValueHandler : NULL, &metaDataHandler);
	if (!SessionSuccess) return (NULL);
	
	if (useValueHandler) {											/* if dynamic value layer is to be created*/
		if (isNewValue) {													/* extra stuff for CMNewValue()...				*/
			
			/* Here we're doing a CMNewValue() -- call the "metadata" handler to get this 		*/
			/* type's metadata.  We can then use that to build a data packet.									*/
			
			metaData = (*(CM_CHAR *(*)(CMType))metaDataHandler)((CMType)type);
			
			dataPacket = createDataPacket(type,metaData,constructorData); /*create data pkt...*/
			if (!SessionSuccess) {																		/* ...if it failed...		*/
				cmDeleteAllDynamicValueLayers(baseValueHdr0,isNewValue);/* ...delete all layers	*/
				return (NULL);																					/* ...abort creation		*/
			}
			
			/* Call the "new value" handler.  It should use the data packet to write data to	*/
			/* its base value.																																*/
			
			success = (*(CMBoolean (*)(CMValue, CMType, CMDataPacket))newValueHandler)((CMValue)baseValueHdr, (CMType)type, (CMDataPacket)dataPacket);
			
			if (dataPacket != NULL) CMfree(dataPacket);								/* free the packet			*/
			newOrUseValueName = "CMNewValue";
		} else {
			newOrUseValueName = "CMUseValue";
			success = true;
		}
		
		/* In all cases we call the "use value" handler. It should read the data written by */
		/* the "new value" handler to create its refCon.  It returns the refCon and a 			*/
		/* methandler pointer	that we will use to get the value operation handler addresses.*/
		/* This is then passed to newDynamicValue() to build the new dynamic value (layer). */
		
		if (success) {
			metaHandler = NULL;											/* "use value" must set this or error			*/
			success = (*(CMBoolean (*)(CMValue, CMType, CMMetaHandler*, CMRefCon*))useValueHandler)((CMValue)baseValueHdr, (CMType)type, &metaHandler, &refCon);
			if (success) {
				if (metaHandler == NULL) {						/* we must get a methandler back!					*/
					typeName = cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName);
					ERROR2(CM_err_NoDynMetahandler, typeName, CONTAINERNAME);
					success = false;
				} else
					baseValueHdr = newDynamicValue(baseValueHdr, type, metaHandler, refCon, newOrUseValueName);
			}
		}
		
		if (!success || baseValueHdr == NULL) {		/* if something went wrong...							*/
			cmDeleteAllDynamicValueLayers(baseValueHdr0, isNewValue); /* ...free the layers		*/
			baseValueHdr = NULL;										/* this will abort the recursion					*/
		}
	}
	
	return (baseValueHdr);											/* return dynamic value layer							*/
}


/*--------------------------------------------------------------*
 | cmGetDynHandlerAddress - get a dynamic value handler address |
 *--------------------------------------------------------------*

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
 always be an API value operation (see   DynValus.h    to see how we use the macros that
 call this routine).
 
 The found handler address and "this" pointer are saved in the passed dynamic value's
 vector entry so we don't have to do the search on successive uses.  The "this" pointer
 and handler address are also saved in the vector entry corresponding to "this" (unless,
 of course, it is the "real" value).
  
 In the vector entry there is also a boolean flag that tells us whether the handler is
 currently active, i.e., in the call chain.  If it is we have a recursion attempt.  This
 causes an error report and NULL to be returned.  Since we will always find the handler
 or use the "real" value, we can never get an error from that.  The recursion is the only
 error condition.
    
 											Caution -- Implementation/portability Note
 
 In order to get at the corresponding dynamic vector in the vector of the final "this"
 value, we convert the original vectorEntry to an offset in the value header extensions,
 i.e., offset to the vector entry within a DynValueHdrExt struct.  This is used to
 "relocate" to the corresponding vector entry.  I point this out only to make you aware of
 this "trick" (s it really a trick?).  It appears to be standard ANSI C because fields in
 a struct are supposed to be ordered by ascending address.  In other words it is NOT
 implementation defined how to lay out fields in a struct (except for alignment).  If your
 compiler thinks otherwise, this will be a problem area.  I just thought you would like to
 know!  
 
 Oh, by the way, the code we're talking about here is near the end of the routine.
*/

CMValue cmGetDynHandlerAddress(CMValue value, DynamicValueVectorEntriesPtr vectorEntry,
															 CMconst_CMGlobalName operationType, CM_CHAR *routineName)
{
	CM_LONG 			entryOffset;
	CMValue 		 	origValue;
	ContainerPtr 	container;
	CMHandlerAddr	handler;

	/* It's an error to attempt a recursive call...																				*/
	
	if (vectorEntry->active) {												/* error if recursing...						*/
		container = ((TOCValueHdrPtr)value)->container;
		ERROR2(CM_err_HandlerRecursed, routineName, CONTAINERNAME);
		return (NULL);
	}
	
	/* We are not recursing and this is not the first time we're using this handler, just	*/
	/* return the "this" pointer associated with the handler we found on an earlier call.	*/
	/* They were initialized to NULL when the vector was allocated.  We cannot test the		*/
	/* handler pointer because it will not be set if the "this" pointer is for a base			*/
	/* "real" value.																																			*/
	
	if (vectorEntry->thisValue != NULL) return (vectorEntry->thisValue);
	
	/* This is the first call to the handler.  We now must search up the dynamic value 		*/
	/* chain, starting with the passed dynamic value, looking for the first dynamic value */
	/* that supplies the handler. The handler is accessed in the usual way via its dynamic*/
	/* value metahandler.	 																																*/
	
	origValue = value;																/* remember starting dynamic value	*/
	
	for (;;) {																				/* loop till we find the handler...	*/
		handler = (CMHandlerAddr)(*DYNEXTENSIONS(value)->metaHandler)(NULL, operationType);
		if (handler != NULL) break;											/* break when we find the handler		*/
		
		/* If not found, we must get the current value's base value.  We can do this until	*/
		/* we reach the original "real" value. If we do reach it, then that stops the search*/
		/* and it is the "real" value we return. In that case, there is no handler because	*/
		/* the calling API value routine will directly operate on the value.  Thus we leave	*/
		/* the handler address in the original value alone.  Of course we set the "this"		*/
		/* pointer.  The next time the dynamic value is used, we will return the "real" 		*/
		/* value.  As discussed in   DynValus.h   , the caller tests the returned value 		*/
		/* to see if it is still a dynamic value.  If it isn't, the handler use is bypassed.*/
		
		value = (CMValue)(DYNEXTENSIONS(value)->baseValue);/* basically a CMGetBaseValue()	*/
		if (!IsDynamicValue(value)) {										/* if we reached the "real" value...*/
			vectorEntry->thisValue = value;								/* save "this" value for next time	*/
			return (value);																/* return the "real" value as "this"*/
		}
	} /* for */
	
	/* Set the "this" pointer and handler address in the original vector entry for the		*/
	/* next time we use it.  Note that we can only exit the above "for" loop if we find 	*/
	/* the handler.  If we loop up to the "real" value without finding it, we exit the 		*/
	/* routine from inside the loop after saving the "this" pointer as we do here.				*/
	
	vectorEntry->thisValue = value;										/* remember "this"									*/
	vectorEntry->handler	 = handler;									/* remember its handler address			*/
	
	/* Just to be safe, we want to copy the "this" pointer and handler address in the			*/
	/* vector entry corresponding to "this". Of course we only need to do this if we 			*/
	/* "inherited" a handler (i.e., we looped up to a base dynamic (but not real) value.	*/
	/* Note, this is where we do the struct offset relocation warned about earlier.				*/
	
	if (value != origValue) {													/* if "inherited"...								*/
		entryOffset = (CM_CHAR *)vectorEntry - (CM_CHAR *)DYNEXTENSIONS(origValue); /* relocate...*/
		vectorEntry  = (DynamicValueVectorEntriesPtr)((CM_CHAR *)DYNEXTENSIONS(value) + entryOffset);
		vectorEntry->thisValue = value;									/* set "this"s vector entry					*/
		vectorEntry->handler	 = handler;
	}
	
	return (value);																		/* return "this"										*/
}


/*-------------------------------------------------------------*
 | cmVScanDataPacketGuts - extract the data from a data packet |
 *-------------------------------------------------------------*
 
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
  
 The metadata format is discussed in the documentation at the start of this file.  The only
 special case is for the "%*s" format specification.  This generates a fixed length string
 (not null terminated) of n characters, where n is determined from a parameter in the
 CMNewValue() "..." paramters.  The value of n is explicitly written to the packet data in
 front of the string.  For symmetry (and so we can determine the number of characters to
 extract from the packet data), we return the value of n to an explict parameter pointer
 passed here.  Thus the parameter pointer list passed to this routine should be identical
 to the "..." parameters passed to a CMNewValue() "..." parameter list (at least the
 portion corresponding to this type).
 
 Here's a word of caution about "%s" -- watch it!  As discussed in note 4 of the format
 specification comments, we COPY the string from the packet to the place pointed to by the
 corresponding dataInitParams parameter.  This is a pitfall (I fell into it, hence these
 comments).  C strings are thought of as pointers (at least I think of them that way). So there is a
 tendency to use "%s" like you would in "printf()".  WRONG!  What you really want is a
 pointer, so use "%p"!
*/

CM_ULONG cmVScanDataPacketGuts(TOCObjectPtr type, CM_CHAR *metaData,
																		CM_UCHAR *dataPacket, va_list dataInitParams)
{
	ContainerPtr 	 container = type->container;
	CM_UCHAR		   *dp, *p;
	CM_CHAR				 *mp, m, *typeName, insert[10];
	CM_ULONG	 		 nbrAssigned;
	CMSize				 n;
	
	/* If there is not data packet or not metaData, there is nothing to do...							*/

	if (dataPacket == NULL || metaData == NULL || *metaData == '\0')
		return (0);
		
	/* Scan the metaData and extract the dataPacket fields according to the that metadata.*/
	/* Each field is put into the location pointed to by the next dataInitParams					*/
	/* parameter.																																					*/
	
	nbrAssigned = 0;																/* count nbr of parameters assigned		*/
	mp = metaData;																	/* mp will be encoding pointer				*/
	dp = dataPacket;																/* each data packet item starts at dp	*/
	
	while ((CMBoolean)(m = *mp++))	{									/* scan the encoding...								*/
		if (m != Meta_Trigger) continue;							/* if not format specification, loop	*/
		
		p = va_arg(dataInitParams, CM_UCHAR *);				/* point at next parameter						*/
		
		switch ((CMBoolean)(m = *mp++)) {
			case Meta_Value:																					/* %v										*/
			case Meta_Object:																					/* %o										*/
			case Meta_Ptr:		memmove(p, dp, sizeof(void *));					/* %p										*/
												dp += sizeof(void *);
												++nbrAssigned;
												continue;
								
			case Meta_Char:		memmove(p, dp, sizeof(CM_UCHAR));	/* %c										*/
												dp += sizeof(CM_UCHAR);
												++nbrAssigned;
												break;
								
			case Meta_Short:	memmove(p, dp, sizeof(CM_USHORT));			/* %d										*/
												dp += sizeof(CM_USHORT);
												++nbrAssigned;
												continue;
			
			case Meta_Long: 	if (*mp == Meta_LongSuffix) ++mp;				/* %l[d]								*/
			case Meta_ID:			memmove(p, dp, sizeof(CM_ULONG));				/* %i										*/
												dp += sizeof(CM_ULONG);
												++nbrAssigned;
												continue;
			
			case Meta_Star:		if ((m = *mp++) != Meta_String) {				/* %*s									*/
													insert[0] = '*';
													insert[1] = m;
													insert[2] = '\0';
													typeName = cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName);
													ERROR3(CM_err_BadMetaSpec, insert, typeName, CONTAINERNAME);
													return (0);
												}
												memmove(&n, dp, sizeof(CMSize));
												memmove(p,  dp, sizeof(CMSize));
												dp += sizeof(CMSize);
												++nbrAssigned;
												p = va_arg(dataInitParams, CM_UCHAR *);
												memmove(p, dp, (size_t)n);
												dp += n;
												++nbrAssigned;
												continue;
												
			case Meta_String:	strcpy((CM_CHAR *)p, (CM_CHAR *)dp);		/* %s										*/
												++nbrAssigned;													/* see caution comments!*/
												continue;
		
			default:					insert[0] = m;													/* oops!								*/
												insert[1] = '\0';
												typeName = cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName);
												ERROR3(CM_err_BadMetaSpec, insert, typeName, CONTAINERNAME);
												return (0);
		} /* switch */
	} /* while */

	return (nbrAssigned);
}
														  
															CM_END_CFUNCTIONS
