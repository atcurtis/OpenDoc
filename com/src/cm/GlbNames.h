/* @(#)Z 1.4 com/src/cm/GlbNames.h, odstorage, od96os2, odos29646d 96/11/15 15:27:12 (96/10/29 09:18:39) */
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
	File:		GlbNames.h

	Contains:	Container Manager Global Name Values Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182319 Matches GlbNames.c.
		 <2>	  5/9/94	MB		#1162181: Changes necessary to install MMM.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                           <<<  GlbNames.h   >>>                           |
 |                                                                           |
 |              Container Manager Global Name Values Interfaces              |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 12/12/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 The routines here handle global names.  Global names are object values that are treated
 speciaially.  Normally object values are written to a container and the value we keep in
 the TOC is the offset to the value (and of course its length).  But we want to keep
 global names around so we can handle such calls as CMGetGlobalName() which return
 the global name value for the object (assuming it is such an object).  We also want to
 efficiently determine if there are attempts at multiple definitions (CMRegister...()) of
 the same global name.
 
 Special flag bits are used in the TOC entry to indicate the value is a global name and
 points to it instead of being an offset.  There is room in the TOC value entry for the
 offset too. It does get filled in when we actually write the global name to the container.
 But the pointer must coexist so we can still get at the name string.  We can get away 
 with this because a TOC value is two longs.  Thus we use one for the offset and one for
 the pointer to the global name.  We therefore have to use the pointer to get at the
 global name to get its length when doing the writing.  But this is getting off the track!
 
 Essentially a global name "belongs" to two data structures. The first is the TOC stucture
 defined in  TOCEnts.h   (or .c).  As just mentioned there is a pointer from the value to
 the global name.  The second data structure is the symbol table which contains the 
 global names themselves.  This is a binary tree.  We can easily walk it when it comes time
 to write the global names out instead of walking the more complicated TOC hunting down
 just the global names (although there is nothing prohibiting that -- it was a time/space
 trade-off).
 
 In addition to the global name in its symbol table entry, there is also a back link to
 the value that points to it.  We can thus go from global name to owning object and vice
 versa.  The back link is used when we write the global names to the container so that we
 can fill in the offset in the object's value as mentioned above.
 
 This is a pretty complicated layout.  There is a potential "trap" in that TOC entries
 can be deleted.  If they are, and it is one that points to a global name, we must delete
 the global name too.  But deletingin in a binary tree is a pain! So what we do is 
 simply mark an entry deleted by nulling out its back pointer.  The trap here is that if
 we're not carful we could end up use a null pointer.  Just thought you would like to 
 know that.
*/

#ifndef __GLOBALNAMES__
#define __GLOBALNAMES__

#include <string.h>
#include <setjmp.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __SYMMGR__
#include "SymTbMgr.h"      
#endif
#ifndef __TOCENTRIES__
#include "TOCEnts.h"   
#endif
#ifndef __TOCOBJECTS__
#include "TOCObjs.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif

																	CM_CFUNCTIONS

struct GlobalName {									/* Layout for a global name symbol table entry:			*/
	SymbolLinks 		theNameLinks;			/* 		standard right/left links (must be 1st)				*/
	struct TOCValue *theValue;				/* 		back ptr to TOCValueBytes (NULL ==> deleted)	*/
	char		globalName[1];						/*		start of this global name											*/
};
typedef struct GlobalName GlobalName, *GlobalNamePtr;


void *cmCreateGlobalNameTbl(ContainerPtr container);
	/*
	This routine MUST be called before any other global name routine is called. It is used to
	set up the data needed by the other global name routines.  A "reference number" is
	returned to the caller who must pass it to all the other global name routines. Any number
	of global name tables can be created.  By passing the returned "reference number" to the
	other routines they will operate on the corresponding table.
	 
	Internally, i.e., in this file, the "reference number" is actually a pointer to a control
	block containing the root pointer to the global name table (set to NULL here) and a use
	count.  The use count is explained in cmUseGlobalNameTbl().
	
	This function returns the pointer if we can allocate the control block. NULL is returned
	if the allocation fails.
	*/
	
	
void *cmUseGlobalNameTbl(void *table, ContainerPtr container);
	/*
	This allows multiple users of the same global name symbol for the specified container. 
	This can come about due to updating containers which want to use the global name table of
	their target.  They call this routine to register the additional use of the specified 
	table.  It is also returned as the function result.
	
	Here we register the additional use by incrementing a use count of the table.  When
	cmFreeAllGlobalNames() is called to remove the global names it will decrement the use
	count.  Only when the count goes to 0 is the data actually freed.
	*/
	

char *cmIsGlobalNameObject(TOCObjectPtr typePropertyObject, CM_ULONG objectID);
	/*
	Given a pointer to a type or property object, which is expected to have the corresponding
	standard global name object ID defined by the macros CM_StdObjID_GlobalTypeName and 
	CM_StdObjID_GlobalPropName, return the pointer to the global name string if the object is
	indeed for a global name.  NULL is returned if the object isn't for a global type or
	property name.
	*/
	
	
char *cmGetGlobalTypeName(ContainerPtr container, CM_ULONG typeID);
	/*
	This routine takes an object ID assumed to be a type object, and returns the global name
	for that type.  A string is always returned.  If the ID does not correspond to a type,
	"?" is returned as the string.
	
	Note, this routine differs from cmIsGlobalNameObject() in that here an object ID is
	converted to its corresponding global name.  In cmIsGlobalNameObject(), it is assumed the
	object has already been found.  So all we have to do here is do the find and then call
	cmIsGlobalNameObject() to get the name.
	*/
	
	
char *cmGetGlobalPropertyName(ContainerPtr container, CM_ULONG propertyID);
	/*
	This routine takes an object ID assumed to be a property object, and returns the global
	name for that property.  A string is always returned.  If the ID does not correspond to a
	type, "?" is returned as the string.
	
	Note, this routine differs from cmIsGlobalNameObject() in that here an object ID is
	converted to its corresponding global name.  In cmIsGlobalNameObject(), it is assumed the
	object has already been found.  So all we have to do here is do the find and then call
	cmIsGlobalNameObject() to get the name.  This is exactly like cmGetGlobalTypeName() but
	for property names instead of types.
	
	Both these routines are mainly used to generate error message inserts.
	*/


#define MarkGlobalNameDeleted(g) ((GlobalNamePtr)(g))->theValue = NULL
	/*
	Marks a global name symbol pointed to by "g" as deleted.  Such an entry will not be
	written to the container.  It is assumed that if there is an object's value pointing to
	this entry, it will never be used (and should itself be deleted).
	*/


#define GetGlobalNameValueBackPtr(g) (((GlobalNamePtr)(g))->theValue)
	/*
	This generates the TOCValue back pointer from a global name symbol table entry pointed to
	by "g".
	*/


#define SetGlobalNameValueBackPtr(g, v) ((GlobalNamePtr)(g))->theValue = (v)
	/*
	This sets the TOCValue back pointer field of a global name symbol table entry pointed to
	by "g" to the value specified by "v".
	*/


#define GetGlobalName(g) (((GlobalNamePtr)(g))->globalName)
	/*
	This generates a pointer to the global name string in a global name symbol table entry
	pointed to by "g".
	*/


#define GetGlobalNameLength(g) (strlen((char *)GetGlobalName(g)))
	/*
	This produces the length of a global name string in a global name symbol table entry
	pointed to by "g".  This length does NOT include the delimiting null at the end of the
	string.
	*/


GlobalNamePtr cmCreateGlobalName(const void *table, const CM_UCHAR *globalName,
																 CMBoolean *dup);
	/*
	Creates and defines a new global name symbol table entry in the specified global name
	table.  A global name symbol table entry consists of the name and a back pointer to the
	TOCValue which will point to the symbol table entry to be able to get the value data. The
	back pointer is set to NULL in the newly defined global name entry returned from here.
	
	The function returns a pointer to the created definition.  If dup is true, a pointer to
	a previously defined entry is returned.  If NULL is returned then there was an allocation
	failure and the new type could not be created.
 
	The back pointer is used (when cmWriteAllGlobalNames() is called) when we write global
	names to the container to allow us to fill in the ACTUAL TOC value data with the
	container offset to the value.	 We make sure we write the global names to the container
	before we write the TOC.  Thus by the time the TOC is written the offsets will be known.
	
	Note, it is assumed that the back pointer will be set by the caller of 
	cmDefineGlobalName().
	*/


GlobalNamePtr cmEnterGlobalName(const void *table, GlobalNamePtr createdGlobalName,
																CMBoolean *dup);
	/*
	This routine takes a previously created global name symbol table entry end enters it into
	the global name table.
 
	The function returns the entry pointer as its result when dup is false. It may NOT be the
	input pointer if we are reusing a previously deleted entry.  In that case the input
	pointer is freed.  
	
	If dup is true, a pointer to a previously defined entry is returned.
	
	This routine differs from cmCreateGlobalName() above in that here no global name entry
	is allocated.  Indeed cmCreateGlobalName() does call this routine after it does its
	allocation.  This routine is also called by buildGlobalNameTable() in  TOCEnts.c   to
	enter read global names into the table.
	
	Caution: The caller should have only entered the name into the entry prior to call.  This
					 is because we could return a pointer to a previously freed entry.
	*/
	
	
GlobalNamePtr cmLookupGlobalName(const void *table, const CM_UCHAR *globalName);
	/*
  Finds the specified global name defined in the specified global name table.  If found,
  the GlobalNamePtr to the found entry is returned.  If not found NULL is returned.
	
	Note, we allocate a temporary global name table entry here and then free it.  If the
	allocation fails, SessionSuccess, a session status switch, is returned false.  Otherwise
	SessionSuccess is true.
	*/


TOCObjectPtr cmDefineGlobalNameObject(const ContainerPtr container, CM_ULONG objectID,
									  									CM_ULONG propertyID, CM_ULONG typeID, 
																			CM_UCHAR *globalName, CM_ULONG generation,
																			CM_USHORT flags, CM_USHORT objectFlags);
	/*
	This routine is called to define an TOC object corresponding to a global name in the
	specified container.  It is a special case of the more general cmDefineObject() routine.
	See that routine for a discussion on the parameters.
 
	The function returns a pointer to the object if it was successfully created and NULL if
	it wasn't.  An error is reported if NULL is returned.  Note, while error calls are not
	supposed to return we assume here they due just to be safe!
	
	The only difference between this routine and cmDefineObject() is that the value is passed
	as a global name string instead of a built TOCValueBytes structure.  Finally, we don't
	bother returning a pointer to the value's header because here we should only have one
	value for the global name.  We can always get at it if we need to through the object
	itself.
	 
	Here we take the string and build a TOCValueBytes value to point to a global name symbol
	table entry which is also created from here.  The symbol table entry will have a back
	pointer to the value entry we create for the object.
	
	The flags and objectFlags are passed just as in cmDefineObject().  However, the flags are
	ORed with the kCMGlobalName flag.  This marks the value as unique and as a value with a
	pointer to a global name symbol table entry. The ValueGlobal flag echos this in the value
	valueHdr.
	
	The main reasons we don't use cmDefineObject() directly is because of the extra work
	needed to be done for global names to set the back pointer in their entries. We can also
	do the extra checks for attempts at multiply defining a global name object.
	*/
	
	
int cmForEachGlobalName(const void *table, CMRefCon refCon,
												void (*action)(GlobalNamePtr globalName, CMRefCon refCon));
	/*
	Do (call) the specified action for each defined global name in the specified global name
	symbol table. The pointer to each global name entry is passed to the action routine along
	with a "refCon" which the caller can use as a communication facility to convey additional
	info to the action routine. 0 is returned to indicate successfull completion.  Use the
	AbortForEachGlobalName(x) (a macro) in the action routine to abort the interator.  The 
	"x" is a integer which is returned from cmForEachGlobalName() so it should not be 0. 
	*/


#define AbortForEachGlobalName(x) longjmp(SESSION->cmForEachGlobalNameEnv, x)
	/*
	Use this in routines passed to forEachGlobalName() to abort the interator. The value of x
	is returned from forEachGlobalName().  It should be non-zero since 0 is the "success"
	return value for forEachGlobalName().  It is assumed that the variable "container" is
	the current container pointer.
	*/


CMBoolean cmWriteAllGlobalNames(const void *table);
	/*
	This routine is called by CMCloseContainer() just prior to writing the TOC to the
	container to make sure all the global names in the specified global name symbol table are
	written to the (end of the) container first.  Global names are thus the last data objects
	in a container before the TOC.
 
	The function returns true if the global names were successfully written and false
	otherwise.  As usual this is as safety since the error handler should not normmaly
	return.
	
	Note, only global names in the specified symbol table that are "owned" by the container
	are written.  A container used to update a target container utilize a global symbol table
	common to both.  However, only NEW global names  that are to be recorded as belonging to
	the updating container are to be written into that container.
	
	We keep global names in memory to make it easier to get at them for such API routines as
	CMGetGlobalName().  Another use is to efficiently determine if there are attempts at
	multiple definitions (CMRegister...()) of the same global name.
	
	The price we pay for this is a complication of TOC entries in that the corresponding 
	value for a global name is NOT a container offset prior to writing.  It is a pointer to
	its global name symbol table entry!  By the time we return from here an offset WILL be
	in the value along with the symbol table pointer.  Funny thing about that!  Just in time
	for the TOC write.
	*/

void cmFreeAllGlobalNames(void **table);
	/*
	This routine is called to remove the definitions of ALL previously defined global names
	in the specified global name symbol table for a container.  The caller's symbol table
	pointer is set to NULL prior to return.
 
  Note, there may be multiple users of a global name symbol table.  Thus its use count is
  passed.  It is decremented and the symbol table only freed if the count goes to 0.
	*/


														  CM_END_CFUNCTIONS
#endif
