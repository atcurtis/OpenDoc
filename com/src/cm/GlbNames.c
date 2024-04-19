/* @(#)Z 1.4 com/src/cm/GlbNames.c, odstorage, od96os2, odos29646d 96/11/15 15:27:09 (96/10/29 09:18:33) */
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
	File:		GlbNames.c

	Contains:	Container Manager Global Name Values Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <4>	 3/24/95	EL		1209355:. Forgot to merge in last change.
		 <3>	 3/24/95	EL		1209355: after writing global name, mark it
													as on disk so it would not be written again
													after flush.
		 <2>	 8/26/94	EL		#1182319 cmLookupGlobalName now won't
													allocate and deallocate memory.
		 <2>	 3/31/94	EL		Don't rewrite global name if it is already
													on disk. #1150214
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                           <<<  GlbNames.c   >>>                           |
 |                                                                           |
 |               Container Manager Global Name Values Routines               |
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


#include <stddef.h>
#include <string.h>
#include <setjmp.h>
#include <stdio.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API__
#include "CMAPI.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
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
#ifndef __GLOBALNAMES__
#include "GlbNames.h"   
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __FREESPACE__
#include "FreeSpce.h" 
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef ____ERRORRPT____
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
#pragma segment GlobalNames
#endif


/* The following is the "control block" that is used to access a global name symbol 		*/
/* table.  This is privately known only to this file.  Outside this is referred to via 	*/
/* an anonymous "void *" pointer.																												*/

struct GlobalNameTbl {							/* Layout for a Global Name Table "control block":	*/
	GlobalNamePtr	globalNameRoot;			/*		ptr to root of global name tree								*/
	ContainerPtr	container;					/*		ptr to "owning" container control block				*/
	CM_SHORT			useCount;						/*		number of distinct users of this table				*/
};																	/* Outside caller's see this as "void *"						*/
typedef struct GlobalNameTbl GlobalNameTbl, *GlobalNameTblPtr;

/* The following is used in this file by cmWriteAllGlobalNames() to represent the 	 		*/
/* refCon used there.  This acts as a communication path while walking through the			*/
/* global names.  See that routine for further details.																	*/

struct WriteGblCommArea{					/* cmWriteAllGlobalNames() communication area layout:	*/
	ContainerPtr container;					/* 		write only names belonging to this container		*/
	CMBoolean 	 reuseFreeSpace;		/*		true ==> try to reuse free space for names			*/
};
typedef struct WriteGblCommArea WriteGblCommArea, *WriteGblCommAreaPtr;


/*--------------------------------------------------------*
 | cmCreateGlobalNameTbl - create a new global name table |
 *--------------------------------------------------------*
 
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

void *cmCreateGlobalNameTbl(ContainerPtr container)
{
	GlobalNameTblPtr t;
	
	t = (GlobalNameTblPtr)CMmalloc(sizeof(GlobalNameTbl));/* a "control block" is born!		*/
	if (t == NULL) return (NULL);													/* well I thought it was born!	*/

	t->globalNameRoot = NULL;
	t->container  		= container;												/* ...set "owning" container		*/
	t->useCount				= 1;																/* 1 user unless cmUse...() done*/
	
	return ((void *)t);																		/* give caller the "refNum"			*/
}


/*-------------------------------------------------------------------------------*
 | cmUseGlobalNameTbl - allow multiple uses of the same global name symbol table |
 *-------------------------------------------------------------------------------*
 
 This allows multiple users of the same global name symbol for the specified container. 
 This can come about due to updating containers which want to use the global name table of
 their target.  They call this routine to register the additional use of the specified 
 table.  It is also returned as the function result.
 
 Here we register the additional use by incrementing a use count of the table.  When
 cmFreeAllGlobalNames() is called to remove the global names it will decrement the use
 count.  Only when the count goes to 0 is the data actually freed.
*/

void *cmUseGlobalNameTbl(void *table, ContainerPtr container)
{
	++((GlobalNameTblPtr)table)->useCount;					 		/* register the additiona use			*/
	((GlobalNameTblPtr)table)->container = container; 	/* set the new owning container		*/
	
	return (table);																			/* caller can now use this TOC		*/
}


/*-----------------------------------------------------------------*
 | enterCompare - type name comparison routine for cmEnterSymbol() |
 *-----------------------------------------------------------------*
 
 This routine is "sent" to cmEnterSymbol() to do the proper comparisons for global names.
 The global name tree is based on the names (obviously).  cmEnterSymbol() is a generic
 binary tree routine that requires the comparsion to be supplied by its caller to decide
 on what basis the tree is build.  Hence this routine!
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmEnterSymbol().
*/

static int enterCompare(const void *g1, const void *g2)
{
	return (strcmp((CM_CHAR *)((GlobalNamePtr)g1)->globalName,
								 (CM_CHAR *)((GlobalNamePtr)g2)->globalName));
}

/*-------------------------------------------------------------------*
 | lookupCompare - type name comparison routine for cmLookupSymbol() |
 *-------------------------------------------------------------------*
 
 This routine is "sent" to cmLookupSymbol() to do the proper comparisons for global names.
 The global name tree is based on the names (obviously).  cmLookupSymbol() is a generic
 binary tree routine that requires the comparsion to be supplied by its caller to decide
 on what basis the tree is build.  Hence this routine!
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmEnterSymbol().
*/

static int lookupCompare(const void *g1, const CM_UCHAR *name)
{
	return (strcmp((CM_CHAR *)((GlobalNamePtr)g1)->globalName, (CM_CHAR *)name));
}


/*----------------------------------------------------------*
 | cmCreateGlobalName - create and define a new global name |
 *----------------------------------------------------------*
 
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

GlobalNamePtr cmCreateGlobalName(const void *table, const CM_UCHAR *globalName,
																 CMBoolean *dup)
{
	GlobalNamePtr g, newGlobalName;
	ContainerPtr	container = ((GlobalNameTblPtr)table)->container;
		
	if ((newGlobalName = (GlobalNamePtr)CMmalloc(sizeof(GlobalName) + strlen((CM_CHAR *)globalName))) != NULL) {
		strcpy((CM_CHAR *)newGlobalName->globalName, (CM_CHAR *)globalName);/* fill in new entry	*/
		newGlobalName->theValue = NULL;																/* set elsewhere			*/
		g = cmEnterGlobalName(table, newGlobalName, dup);							/* enter the name			*/
	} else {
		g = NULL;
		*dup = false;
	}
	
	return (g);																											/* return entry ptr		*/
}


/*---------------------------------------------------------------*
 | cmEnterGlobalName - define an already created new global name |
 *---------------------------------------------------------------*
 
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

GlobalNamePtr cmEnterGlobalName(const void *table, GlobalNamePtr createdGlobalName,
																CMBoolean *dup)
{
	GlobalNamePtr g;
	ContainerPtr	container = ((GlobalNameTblPtr)table)->container;
		
	g = (GlobalNamePtr)cmEnterSymbol(createdGlobalName, (void **)&((GlobalNameTblPtr)table)->globalNameRoot, 
																	 dup, enterCompare);
	
	if (*dup) {																										/* possible dup...			*/
		CMfree(createdGlobalName);																	/* we don't need new one*/
		if (g->theValue == NULL)																		/* if "deleted"...			*/
			*dup = false;																							/* ..it's really not dup*/
	} 
	
	return (g);																										/* return entry ptr			*/
}


/*------------------------------------------------------------*
 | cmLookupGlobalName - find a previously defined global name |
 *------------------------------------------------------------*
 
 Finds the specified global name defined in the specified global name table.  If found,
 the GlobalNamePtr to the found entry is returned.  If not found NULL is returned.
 
 Note, we allocate a temporary global name table entry here and then free it.  If the
 allocation fails, SessionSuccess, a session status switch, is returned false.  Otherwise
 SessionSuccess is true.
*/

GlobalNamePtr cmLookupGlobalName(const void *table, const CM_UCHAR *globalName)
{
	GlobalNamePtr g;
	ContainerPtr	container = ((GlobalNameTblPtr)table)->container;
	
	g = (GlobalNamePtr)cmLookupSymbol(globalName, ((GlobalNameTblPtr)table)->globalNameRoot, lookupCompare);
	if (g && g->theValue == NULL) g = NULL;											/* chk for deleted name	*/
	SessionSuccess = true;
	
	return (g);
}


/*------------------------------------------------------------------------*
 | cmDefineGlobalNameObject - define an object representing a global name |
 *------------------------------------------------------------------------*
 
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
 needed to be done for global names to set the back pointer in their entries.  We can also
 do the extra checks for attempts at multiply defining a global name object.
*/

TOCObjectPtr cmDefineGlobalNameObject(const ContainerPtr container, CM_ULONG objectID,
									   									CM_ULONG propertyID, CM_ULONG typeID, 
										 									CM_UCHAR *globalName, CM_ULONG generation,
																			CM_USHORT flags, CM_USHORT objectFlags)

{
	TOCValueBytes	 valueBytes, *v;
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr		 theValue;
	TOCObjectPtr	 theObject;
	char					 idStr[15];
	
	/* Fill in our value bytes with the pointer to the global name symbol table entry...	*/
	
	v = cmSetValueBytes(container, &valueBytes, Value_GlobalName, (CM_ULONG)globalName, 0);
	if (v == NULL) return (NULL);
	
	/* Create the object (hopefully).  The returned theValueHdr will be the header to the	*/
	/* object's value chain.  There must be only one value or we have an error.	If we're	*/
	/* happy we can set the back pointer in the global name entry.												*/
	
	theObject = cmDefineObject(container, objectID, propertyID, typeID, &valueBytes,
													 	 generation, (CM_USHORT)(flags | kCMGlobalName),
														 objectFlags, &theValueHdr);
													 
	if (theObject != NULL)
		if (cmCountListCells(&theObject->propertyList) > 1 ||
				cmCountListCells(&((TOCPropertyPtr)cmGetListHead(&theObject->propertyList))->valueHdrList) > 1) {
			ERROR3(CM_err_MultGlblNames, cmltostr(theObject->objectID, 1, false, idStr),
																	 globalName, CONTAINERNAME);
			theObject = NULL;
		} else {
			theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList);	/* tail if u want	*/
			SetGlobalNameValueBackPtr(theValue->value.globalName.globalNameSymbol, theValue);
			if ((objectFlags & ProtectedObject) != 0)				/* if object is protected....			*/
				theValueHdr->valueFlags |= ValueProtected;		/* ...then protect its value too	*/
		}
	
	return (theObject);											/* return ptr to the created object						*/
}


/*----------------------------------------------------------*
 | cmForEachGlobalName - do some action on each global name |
 *----------------------------------------------------------*
 
 Do (call) the specified action for each defined global name in the specified global name
 symbol table. The pointer to each global name entry is passed to the action routine along
 with a "refCon" which the caller can use as a communication facility to convey additional
 info to the action routine. 0 is returned to indicate successfull completion.  Use the
 AbortForEachGlobalName(x) (a macro) in the action routine to abort the interator.  The 
 "x" is a integer which is returned from cmForEachGlobalName() so it should not be 0. 
*/

int cmForEachGlobalName(const void *table, CMRefCon refCon,
												void (*action)(GlobalNamePtr globalName, CMRefCon refCon))
{
	ContainerPtr container = ((GlobalNameTblPtr)table)->container;
	
	if (setjmp(SESSION->cmForEachGlobalNameEnv))			/* if longjmp taken...							*/
		return (1);																			/* ...report the bad news						*/
		
	cmForEachSymbol(((GlobalNameTblPtr)table)->globalNameRoot, refCon, (SymbolAction)action);
	
	return (0);
}


/*---------------------------------------------------------*
 | write1GlobalName - write a global name to its container |
 *---------------------------------------------------------*
 
 This routine is the action routine for cmWriteAllGlobalNames() to write one global name
 string to its container.  We only write the name if the container field in the value
 segment entry for the global name equals the container pointer in the refCon.  This was
 passed as the updatingContainer to cmWriteAllGlobalNames().
 
 Global name strings are NOT written when they are created (by  cmDefineGlobalName()). They
 are kept in as global name symbol table entries with an object's value data (TOCValue)
 pointing to a symbol table entry for global data objects.  When the object is created
 the global name entry is set with a back pointer to the object's TOCValue value.
 
 Using the back pointer we can get at all the structures of the object and its container.
 Thus we write out the global name string to its container and SET the data value in the
 objects TOCValue value to the offset to where we wrote the string in the container.
 
 The global names are written to the container before we write the TOC.  Thus by the time
 the TOC is written it will have all the offset correct for the global names.  The TOC
 value writer will still have to access the global name entry to get the length which is
 also part of the TOC.  Thus the TOCValue value data for global name will, after we
 complete the global name writing contains two pieces of information: the container offset
 and its global name symbol symbol table pointer.
 
 Note, if a value for an object has been deleted, then the back pointer is set to NULL to
 indicate that fact.  It's a pain to delete entries from a binary tree, so marking an
 entry as deleted is simpler.  We will see the deleted entries as we walk the symbol table
 tree.  The check for the deleted entries must therefore be made.
 
 The only flaw in this is that a newly created, but not completed entry from a call to
 cmDefineGlobalName() also inits the back pointer to NULL. If used correctly (ha!) caller's
 to cmDefineGlobalName() are expected to fill in the back pointer.  Things should go as
 expected.  But if they don't...
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmForEachGlobalName().
*/

static void write1GlobalName(GlobalNamePtr globalNameSymbol, CMRefCon refCon)
{
	WriteGblCommAreaPtr g = (WriteGblCommAreaPtr)refCon;
	TOCValuePtr		 			theValue = globalNameSymbol->theValue;
	TOCValueHdrPtr 			theValueHdr;
	TOCObjectPtr	 			theObject;
	ContainerPtr 	 			container;
	CM_ULONG	 					nameLength, offset, actualSize;
	
	if (theValue == NULL) return;											/* exit if name was deleted					*/

	if (theValue->container != g->container) return;	/* exit if name is not desired			*/
	
	theValueHdr = theValue->theValueHdr;
	container   = theValueHdr->container->updatingContainer;

	if ((theValueHdr->valueFlags & ValueGlobal) == 0) {
		ERROR1(CM_err_NotGlobalName, CONTAINERNAME);
		AbortForEachGlobalName(CM_err_NotGlobalName);
	}
	
	if (theValueHdr->valueRefCon) {							/* has a TOC means already on disk	*/
		return;
	}
	/* Don't write standard object names...																								*/
	
	theObject = theValueHdr->theProperty->theObject;
	if (theObject->objectID > CM_StdObjID_TOC && theObject->objectID < MinUserObjectID)
		return;
		
	/* If we are reusing free space for updating, try to get offset from free list.  If		*/
	/* we are not updating, the seek to the end of the container was alread done.					*/
	
	nameLength  = strlen((CM_CHAR *)globalNameSymbol->globalName) + 1;
	
	if (g->reuseFreeSpace) {													/* if we are to reuse free space...	*/
		offset = cmGetFreeListEntry(container, nameLength, true, &actualSize); /* do it			*/
		if (actualSize != 0)														/* if we got some to reuse...				*/
			CMfseek(container, offset, kCMSeekSet);				/* ...position to write over it			*/
		else { 																					/* if couldn't find a fit...				*/
			CMfseek(container, 0, kCMSeekEnd);						/* ...write data to end of container*/
			offset = container->physicalEOF;							/* used to update highest written		*/
		}
	} else {
		actualSize = 0;																	/* use as switch to update eof info	*/
		offset = container->physicalEOF;								/* used to update highest written		*/
	}
	
	/* Write the global name as a single segment...																				*/
	
	if (CMfwrite(container, globalNameSymbol->globalName, sizeof(CM_UCHAR), nameLength) != nameLength) {
		ERROR2(CM_err_BadGNameWrite, globalNameSymbol->globalName, CONTAINERNAME);
		AbortForEachGlobalName(CM_err_BadGNameWrite);
	}
		
	/* define offset for global name, extra offset if merging															*/
	theValue->value.globalName.offset = offset + container->mergeInSize;
	
	offset += nameLength;															/* update logical OR physical EOF		*/
	if (actualSize == 0)
		container->physicalEOF = offset;								/* update EOF for next global name	*/
	SetLogicalEOF(offset);														/* set logical EOF (may != physical)*/

	theValueHdr->valueRefCon = container->toc;				/* remember it is now on disk				*/	
}


/*-------------------------------------------------------------------*
 | cmWriteAllGlobalNames - write all global names to their container |
 *-------------------------------------------------------------------*
 
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
 
CMBoolean cmWriteAllGlobalNames(const void *table)
{
	WriteGblCommArea gblCommArea;
	ContainerPtr		 container = ((GlobalNameTblPtr)table)->container;
	
	gblCommArea.container = container;											/* write only names from this	*/
	
	if ((container->useFlags & kCMReuseFreeSpace) == 0) {		/* if not reusing free space..*/
		CMfseek(container, 0, kCMSeekEnd);										/* ..write to end of container*/
		gblCommArea.reuseFreeSpace = false;										/* ...suppress seeks					*/
	} else																									/* if updating, resue space...*/
		gblCommArea.reuseFreeSpace = (CMBoolean)((container->useFlags & kCMReuseFreeSpace) != 0);

	container->physicalEOF = CMgetContainerSize(container);
	return ((CMBoolean)(cmForEachGlobalName(table, (CMRefCon)&gblCommArea, write1GlobalName) == 0));
}


/*----------------------------------------------*
 | cmFreeAllGlobalNames - free all global names |
 *----------------------------------------------*
 
 This routine is called to remove the definitions of ALL previously defined global names
 in the specified global name symbol table for a container.  The caller's symbol table
 pointer is set to NULL prior to return.
 
 Note, there may be multiple users of a global name symbol table. The use count is
 decremented and the symbol table only freed if the count goes to 0.
*/

void cmFreeAllGlobalNames(void **table)
{
	ContainerPtr container = ((GlobalNameTblPtr)*table)->container;

	if (--((GlobalNameTblPtr)*table)->useCount > 0) 			/* don't free if multiple users	*/
		return; 

	cmFreeAllSymbols((void **)&((GlobalNameTblPtr)*table)->globalNameRoot, SESSION);

	CMfree(*table);																				/* bye, bye table								*/
	*(GlobalNameTblPtr **)table = NULL;										/* kill caller's table pointer	*/
}


/*------------------------------------------------------------------------------------*
 | cmIsGlobalNameObject - determine if a type or property object is for a global name |
 *------------------------------------------------------------------------------------*
 
 Given a pointer to a type or property object, which is expected to have the corresponding
 standard global name object ID defined by the macros CM_StdObjID_GlobalTypeName and 
 CM_StdObjID_GlobalPropName, return the pointer to the global name string if the object is
 indeed for a global name.  NULL is returned if the object isn't for a global type or
 property name.

 This routine only returns the pointer to the global name string because that is currently
 all we want to know from the object.  If it later turns out we need the actual global
 name entry then all that needs to be changed is the return pointer.  Of course then 
 callers after only the name string will have to do the access.
*/

CM_CHAR *cmIsGlobalNameObject(TOCObjectPtr typePropertyObject, CM_ULONG objectID)
{
	TOCPropertyPtr theProperty;
	TOCValueHdrPtr theValueHdr;
	TOCValuePtr		 theValue;
	
	/* Check everything that can possibly be checked to make sure we indeed do have a 		*/
	/* global name.  Here's what we must have to convenience ourselves of that fact:			*/

	/*		(1). The property with the specified objectID must be part of the object.				*/
	/*		(2). There must be a value header defined for the property.											*/
	/*		(3). The type ID in that value header must be CM_StdObjID_7BitASCII.						*/
	/*		(4). There must be a value for the value header.																*/
	/*		(5). The flags in that value must contain kCMGlobalName.												*/

	/* If we get through all that, we're happy!																						*/
	
	if ((theProperty = cmGetObjectProperty(typePropertyObject, objectID)) != NULL)
		if ((theValueHdr = (TOCValueHdrPtr)cmGetListHead(&theProperty->valueHdrList)) != NULL)
			if (theValueHdr->typeID == CM_StdObjID_7BitASCII)
				if ((theValue = (TOCValuePtr)cmGetListHead(&theValueHdr->valueList)) != NULL)
					if ((theValue->flags & kCMGlobalName) == 0)
						theValue = NULL;
	
	if (theProperty == NULL || theValueHdr == NULL || theValue == NULL)
		return (NULL);
		
	return (GetGlobalName(theValue->value.globalName.globalNameSymbol));
}


/*-------------------------------------------------------------*
 | cmGetGlobalTypeName - return global name for type object ID |
 *-------------------------------------------------------------*
 
 This routine takes an object ID assumed to be a type object, and returns the global name
 for that type.  A string is always returned.  If the ID does not correspond to a type,
 "?" is returned as the string.
 
 Note, this routine differs from cmIsGlobalNameObject() in that here an object ID is
 converted to its corresponding global name.  In cmIsGlobalNameObject(), it is assumed the
 object has already been found.  So all we have to do here is do the find and then call
 cmIsGlobalNameObject() to get the name.
*/
 
CM_CHAR *cmGetGlobalTypeName(ContainerPtr container, CM_ULONG typeID)
{
	TOCObjectPtr 	type;
	CM_CHAR				*typeName;

	type = cmFindObject(container->toc, (CMObjectID)typeID);
	
	typeName = (type == NULL) ? NULL : cmIsGlobalNameObject(type, CM_StdObjID_GlobalTypeName);
	
	if (typeName == NULL || *typeName == '\0') typeName = "?";
	
	return (typeName);
}


/*---------------------------------------------------------------------*
 | cmGetGlobalPropertyName - return global name for property object ID |
 *---------------------------------------------------------------------*
 
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

CM_CHAR *cmGetGlobalPropertyName(ContainerPtr container, CM_ULONG propertyID)
{
	TOCObjectPtr 	property;
	CM_CHAR				*propertyName;

	property = cmFindObject(container->toc, (CMObjectID)propertyID);
	
	propertyName = (property == NULL) ? NULL : cmIsGlobalNameObject(property, CM_StdObjID_GlobalPropName);
	
	if (propertyName == NULL || *propertyName == '\0') propertyName = "?";
	
	return (propertyName);
}
														  
															CM_END_CFUNCTIONS
