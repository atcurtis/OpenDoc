/* @(#)Z 1.4 com/src/cm/CMDbgOps.c, odstorage, od96os2, odos29646d 96/11/15 15:26:45 (96/10/29 09:16:30) */
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
	File:		 CMDbgOps.c 

	Contains:	Container Manager Debugging Operations

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1992-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 12/9/94	EL		#1182275 Optionally do not maintain
									continue flag.
		 <2>	 8/26/94	EL		#1181622 Ownership update
		 <1>	  2/3/94	EL		first checked in
		 <2>	11/23/93	EL		When updating save TOC data so that TOC
									don't need to be read twice.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                           <<<  CMDbgOps.c  >>>                            |
 |                                                                           |
 |                  Container Manager Debugging Operations                   |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 4/23/92                                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1992-1994               |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 This file defines a set of special debugging API routines.  They are kept separate from
 the API (and undocumented) since they may or may not be supported in a particular
 installation.
 
 There are three routines:
 
 	1. CMDebugging()					- to set some internal debugging "options"
	2. CMDumpTOCStructures()	- to dump in-memory TOC as a tree-like format
	3. CMDumpContainerTOC()		- to read in container TOC and display it in a table format

 The CMDebugging() routine is under control of the CMDEBUGGING switch macro.  The 
 installer may suppress this functionality by setting this macro to 0 (its default).
 
 Similarly, the other routines are under control of the CMDUMPTOC switch.
 
 The switches are defined in CMTypes.h.
*/


#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_DEBUG__
#include "CMAPIDbg.h"
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
#ifndef __TOCIO__
#include "TOCIO.h"
#endif
#ifndef __CONTAINEROPS__
#include "Containr.h"  
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
#ifndef __UPDATING__
#include "Update.h"  
#endif
#ifndef __REFERENCES__
#include "Refs.h"      
#endif
#ifndef __ERRORRPT__
#include "ErrorRpt.h"      
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
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
#pragma segment Debugging
#endif


#if CMDEBUGGING || CMDUMPTOC
/*------------------------------*
 | display - isolate all output |
 *------------------------------*
 
 In some environments it may be more desirable (or necessary) to handle I/O specially. To
 that end this routine is provided.  All the debug printing goes through  here.  It's
 parameters are identical to fprintf(), with the same meaning.  So, as delivered, in its
 simplest form, this routine maps into a fprintf().  Feel free to "warp" this routine into
 somthing appropriate to your system.
*/

static void CM_NEAR CM_C display(FILE *stream, const CM_CHAR *format, ...)
{
	va_list ap;
	
	va_start(ap, format);
	vfprintf(stream, format, ap);
	va_end(ap);
}
#endif


#if CMDEBUGGING
/*------------------------------------------------*
 | CMDebugging - set internal debugging "options" |
 *------------------------------------------------*
 
 This is an undocumented (except for what you're reading now) routine to allow me (the
 idiot who got stuck coding this stuff) to do some debugging.  It is called AFTER 
 CMStartSession() but BEFORE any containers are opened.  It takes the session global data
 pointer returned by CMStartSession() and adds (actually modifies) some of the data there
 to enable debugging.
 
 The current data that can be set is:
 
 		1. The size of the TOC index tables.  If 0 is passed, the default is used.  The
			 current implementation of the TOC uses an index table mechanism with the tables
			 each being tocTableSize entries long.  See  TOCObjs.c   and .h for further details.
		
		2. A FILE* debugging output variable.  It is usually stdout, stderr, or NULL.  For NULL
			 no debugging output is produced.  Currently this thing is used to display the TOC as
			 a table when it is open for input.
			 
		3. The dynamic switch controlling the refNum validations (e.g., checking for NULL) and
			 various other protections.  If set to 0 the protection checks are suppressed.  If
			 nonzero they are performed, but only if the protection code is present.  On a per-
			 installation basis it may be suppressed.  In that case the switch has no effect.
 
 The entire routine may be suppressed, which it is by default, by setting the macro
 variable CMDEBUGGING to 0.
*/

void CM_FIXEDARGS CMDebugging(CMSession sessionData, CM_USHORT tocTableSize, 
															FILE CM_PTR *debuggingFile, CMBoolean doValidations)
{
	CM_CHAR s1[10], s2[10];
	#if !CMVALIDATE
	CMBoolean unused = doValidations;
	#endif
	
	if (sessionData != NULL) {
		((SessionGlobalDataPtr)sessionData)->cmDbgFile		= debuggingFile;							
		((SessionGlobalDataPtr)sessionData)->cmTocTblSize = (CM_ULONG)tocTableSize;
		
		if (tocTableSize == 0)
			((SessionGlobalDataPtr)sessionData)->cmTocTblSize = DefaultIndexTableSize;
		else if (tocTableSize < MinIndexTableSize || tocTableSize > MaxIndexTableSize) {
			SessionERROR2(CM_err_BadTocSize, cmltostr(MinIndexTableSize, 1, false, s1),
											 								 cmltostr(MaxIndexTableSize, 1, false, s2));
			((SessionGlobalDataPtr)sessionData)->cmTocTblSize = DefaultIndexTableSize;
		}
		
		#if CMVALIDATE
		((SessionGlobalDataPtr)sessionData)->validate = doValidations;
		#endif
	}
}
#endif


#if CMDUMPTOC
/*----------------------------------------------------*
 | showTouchedList - display an object's touched list |
 *----------------------------------------------------*
 
 This is called by showObject() if there is a updating touch list for that object.  The
 entire touch list is displayed.
*/

static void showTouchedList(TouchedListEntryPtr touch, CMRefCon refCon)
{
	FILE				 	 *f = (FILE *)refCon;
	CM_USHORT			 i, mask, touchFlags;
	CM_CHAR  			 flagsMeaning[256];
	
	display(f, "\n                           --- Touched List ---\n");
	
	while (touch) {
		touchFlags = touch->touchFlags;
		
		if (touchFlags == 0x0000U) 
			*flagsMeaning = '\0';
		else {
			strcpy(flagsMeaning, " (");
			
			for (i = 0, mask = 1; i < 16; ++i, mask <<= 1) {
				if ((touchFlags & mask) == TouchedRemoved) {
					strcat(flagsMeaning, "TouchedRemoved/");
					continue;
				}
				if ((touchFlags & mask) == TouchedInserted) {
					strcat(flagsMeaning, "TouchedInserted/");
					continue;
				}
				if ((touchFlags & mask) == TouchedDeletedValue) {
					strcat(flagsMeaning, "TouchedDeletedValue/");
					continue;
				}
				if ((touchFlags & mask) == TouchedDeletedProperty) {
					strcat(flagsMeaning, "TouchedDeletedProperty/");
					continue;
				}
				if ((touchFlags & mask) == TouchedEdited) {
					strcat(flagsMeaning, "TouchedEdited/");
					continue;
				}
				if ((touchFlags & mask) == TouchedImmediate) {
					strcat(flagsMeaning, "TouchedImmediate/");
					continue;
				}
				if ((touchFlags & mask) == TouchedBaseType) {
					strcat(flagsMeaning, "TouchedBaseType/");
					continue;
				}
				if ((touchFlags & mask) == TouchedSetinfoed) {
					strcat(flagsMeaning, "TouchedSetinfoed/");
					continue;
				}
			} /* for */
			
			strcpy(flagsMeaning + strlen(flagsMeaning) - 1, ")");
		}
		
		display(f, "\n            0x%.8lX: theValueHdr  = 0x%.8lX\n"
							 "                        touchFlags	 = 0x%.4lX%s\n"
							 "                        OPT          = 0x%.8lX/0x%.8lX/0x%.8lX = %ld/%ld/%ld\n"
							 "                        removedEntry = 0x%.8lX\n",
							 touch,
							 touch->theValueHdr,
							 touch->touchFlags, flagsMeaning,
							 touch->objectID, touch->propertyID, touch->typeID, touch->objectID, touch->propertyID, touch->typeID,
							 touch->removedEntry);
		
		touch = (TouchedListEntryPtr)cmGetNextListCell(touch);
	} /* while */
	
	display(f, "\n                        --- end of touched list ---\n");
}

 
/*-----------------------------------------------------------------*
 | showObject - action routine for displaying an object in the TOC |
 *-----------------------------------------------------------------*
 
 This is a debugging routine used as the "action" parameter to cmWalkThroughEntireTOC()
 calls to display selected info for an object in the TOC. The "refCon" here is expected to
 be a file variable.
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmWalkThroughEntireTOC().
*/

static TOCWalkReturns showObject(ContainerPtr container, TOCObjectPtr theObject, CMRefCon refCon)
{
	ContainerPtr 	 unused = container;
	FILE				 	 *f = (FILE *)refCon;
	CM_USHORT			 i, mask, objectFlags;
	CM_CHAR  			 flagsMeaning[256];
	
	objectFlags  = theObject->objectFlags;
	
	if (objectFlags == 0x0000U) 
		*flagsMeaning = '\0';
	else {
		strcpy(flagsMeaning, " (");
		
		for (i = 0, mask = 1; i < 16; ++i, mask <<= 1) {
			if ((objectFlags & mask) == UndefinedObject) {
				strcat(flagsMeaning, "UndefinedObject/");
				continue;
			}
			if ((objectFlags & mask) == ObjectObject) {
				strcat(flagsMeaning, "ObjectObject/");
				continue;
			}
			if ((objectFlags & mask) == PropertyObject) {
				strcat(flagsMeaning, "PropertyObject/");
				continue;
			}
			if ((objectFlags & mask) == TypeObject) {
				strcat(flagsMeaning, "TypeObject/");
				continue;
			}
			if ((objectFlags & mask) == DeletedObject) {
				strcat(flagsMeaning, "DeletedObject/");
				continue;
			}
			if ((objectFlags & mask) == ProtectedObject) {
				strcat(flagsMeaning, "ProtectedObject/");
				continue;
			}
			if ((objectFlags & mask) == LinkedObject) {
				strcat(flagsMeaning, "LinkedObject/");
				continue;
			}
			if ((objectFlags & mask) == UndefObjectCounted) {
				strcat(flagsMeaning, "UndefObjectCounted/");
				continue;
			}
			if ((objectFlags & mask) == DynamicValuesObject) {
				strcat(flagsMeaning, "DynamicValuesObject/");
				continue;
			}
		} /* for */
		
		strcpy(flagsMeaning + strlen(flagsMeaning) - 1, ")");
	}
	
	display(f, "---------------------------------------------\nTOCObject\n");
	display(f, "0x%.8lX: objectID          = 0x%.8lX = %ld\n"
							"            container         = 0x%.8lX (%s) [%d]\n"
							"            nextObject        = 0x%.8lX\n"
							"            prevObject        = 0x%.8lX\n"
							"            nextTypeProperty  = 0x%.8lX\n"
							"            prevTypeProperty  = 0x%.8lX\n"
							"            nextTouchedObject = 0x%.8lX\n"
							"            objectFlags       = 0x%.4X%s\n",
							theObject,
							theObject->objectID, theObject->objectID,
							theObject->container, CONTAINERNAMEx(theObject->container), theObject->container->depth,
							theObject->nextObject,
							theObject->prevObject,
							theObject->nextTypeProperty,
							theObject->prevTypeProperty,
							theObject->nextTouchedObject,
							theObject->objectFlags, flagsMeaning);
							
	if (!cmIsEmptyList(&theObject->touchedList))
		showTouchedList((TouchedListEntryPtr)cmGetListHead(&theObject->touchedList), refCon);
	
	return (WalkNextTOCProperty);
}


/*---------------------------------------------------------------------*
 | showProperty - action routine for displaying an property in the TOC |
 *---------------------------------------------------------------------*
 
 This is a debugging routine used as the "action" parameter to cmWalkThroughEntireTOC() 
 calls to display selected info for a property in the TOC. The "refCon" here is expected
 to be a file variable.
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmWalkThroughEntireTOC().
*/

static TOCWalkReturns showProperty(ContainerPtr container, TOCPropertyPtr theProperty, CMRefCon refCon)
{
	ContainerPtr unused = container;	
	FILE				 *f = (FILE *)refCon;
	
	display(f, "\n            TOCProperty\n"
						 "            0x%.8lX: propertyID = 0x%.8lX = %ld\n"
						 "                        theObject  = 0x%.8lX\n",
						 theProperty,
						 theProperty->propertyID, theProperty->propertyID,
						 theProperty->theObject);

	return (WalkNextTOCValueHdr);
}


/*------------------------------------------------------------------------*
 | showValueHdr - action routine for displaying a value header in the TOC |
 *------------------------------------------------------------------------*
 
 This is a debugging routine used as the "action" parameter to cmWalkThroughEntireTOC()
 calls to display selected info for a value header in the TOC. The "refCon" here is
 expected to be a file variable.
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmWalkThroughEntireTOC().
*/

static TOCWalkReturns showValueHdr(ContainerPtr container, TOCValueHdrPtr theValueHdr, CMRefCon refCon)
{
	FILE				 	 *f = (FILE *)refCon;
	CM_USHORT			 i, mask, valueFlags;
	CM_CHAR				 *extensions, *refs, *shadowListStatus = "", flagsMeaning[256];
	ContainerPtr 	 unused = container;
	#if CMSHADOW_LIST
	RefDataShadowEntryPtr r = NULL;
	#endif
	
	valueFlags = theValueHdr->valueFlags;
	
	if (valueFlags & ValueDynamic)
		extensions = "extensions   ";
	else
		extensions = "dynValue     ";
	
	#if CMSHADOW_LIST
	if (HasRefDataObject(theValueHdr))
		refs = "refDataObject";
	else if (theValueHdr->typeID == CM_StdObjID_ObjRefData) {
		refs = "refShadowList";
		if (RefShadowList(theValueHdr) != NULL) {
			r = (RefDataShadowEntryPtr)cmGetListHead(RefShadowList(theValueHdr));
			shadowListStatus = (r == NULL) ? " (<empty>)" : "";
		}
	} else
		refs = "refDataObject";
	#else
	refs = "refDataObject";
	#endif
	
#if !CMKEEP_CONTINUE_FLAG
	if (cmCountListCells(&theValueHdr->valueList) > 1)
		valueFlags |= ValueContinued;
	else
		valueFlags &= ~ValueContinued;
#endif

	if (valueFlags == 0x0000U)
		*flagsMeaning = '\0';
	else {
		strcpy(flagsMeaning, " (");
		
		for (i = 0, mask = 1; i < 16; ++i, mask <<= 1) {
			if ((theValueHdr->valueFlags & mask) == ValueDeleted) {
				strcat(flagsMeaning, "ValueDeleted/");
				continue;
			}
			if ((theValueHdr->valueFlags & mask) == ValueContinued) {
				strcat(flagsMeaning, "ValueContinued/");
				continue;
			}
			if ((theValueHdr->valueFlags & mask) == ValueGlobal) {
				strcat(flagsMeaning, "ValueGlobal/");
				continue;
			}
			if ((theValueHdr->valueFlags & mask) == ValueImmediate) {
				strcat(flagsMeaning, "ValueImmediate/");
				continue;
			}
			if ((theValueHdr->valueFlags & mask) == ValueOffPropChain) {
				strcat(flagsMeaning, "ValueOffPropChain/");
				continue;
			}
			if ((theValueHdr->valueFlags & mask) == ValueDynamic) {
				strcat(flagsMeaning, "ValueDynamic/");
				continue;
			}
			if ((theValueHdr->valueFlags & mask) == ValueUndeletable) {
				strcat(flagsMeaning, "ValueUndeletable/");
				continue;
			}
			if ((theValueHdr->valueFlags & mask) == ValueProtected) {
				strcat(flagsMeaning, "ValueProtected/");
				continue;
			}
			if ((theValueHdr->valueFlags & mask) == ValueDefined) {
				strcat(flagsMeaning, "ValueDefined/");
				continue;
			}
		} /* for */
		
		strcpy(flagsMeaning + strlen(flagsMeaning) - 1, ")");
	}
	
	display(f, "\n                        TOCValueHdr\n"
						 "                        0x%.8lX: typeID        = 0x%.8lX = %ld\n"
						 "                                    theProperty   = 0x%.8lX\n"
						 "                                    container     = 0x%.8lX (%s) [%d]\n"
						 "                                    size          = 0x%.8lX = %ld\n"
						 "                                    logicalSize   = 0x%.8lX = %ld\n"
						 "                                    generation    = 0x%.8lX\n",
						 theValueHdr,
						 theValueHdr->typeID, theValueHdr->typeID,
						 theValueHdr->theProperty,
						 theValueHdr->container, CONTAINERNAMEx(theValueHdr->container), theValueHdr->container->depth,
						 theValueHdr->size, theValueHdr->size,
						 theValueHdr->logicalSize, theValueHdr->logicalSize,
						 theValueHdr->generation);
	display(f, "                                    useCount      = 0x%.8lX\n"
						 "                                    valueRefCon   = 0x%.8lX\n"
						 "                                    touch         = 0x%.8lX\n"
						 "                                    %s = 0x%.8lX\n"
						 "                                    %s = 0x%.8lX%s\n"
						 "                                    valueFlags    = 0x%.4X%s\n",
						 theValueHdr->useCount,
						 theValueHdr->valueRefCon,
						 theValueHdr->touch,
						 extensions, DYNEXTENSIONS(theValueHdr),
						 refs, RefDataObject(theValueHdr), shadowListStatus,
						 theValueHdr->valueFlags, flagsMeaning);
	
	if (theValueHdr->valueFlags & ValueDynamic)
		display(f, "                        Extensions: baseValue     = 0x%.8lX\n",
							 DYNEXTENSIONS(theValueHdr)->baseValue);
	
	#if CMSHADOW_LIST
	if (r != NULL) {
		display(f, "\n                                                    Key      Object ID\n"
							 "                        Shadow List: 0x%.8lX: 0x%.8lX  0x%.8lX = %ld\n",
							 r, r->key, r->objectID, r->objectID);
		r = (RefDataShadowEntryPtr)cmGetNextListCell(r);
		if (r == NULL)
			display(f, "                        0x%.8lX\n", RefShadowList(theValueHdr));
		else {
			display(f, "                        0x%.8lX   0x%.8lX: 0x%.8lX  0x%.8lX = %ld\n",
								 RefShadowList(theValueHdr), r, r->key, r->objectID, r->objectID);
			r = (RefDataShadowEntryPtr)cmGetNextListCell(r);
		}
		while (r) {
			display(f, "                                     0x%.8lX: 0x%.8lX  0x%.8lX = %ld\n",
								 r, r->key, r->objectID, r->objectID);
			r = (RefDataShadowEntryPtr)cmGetNextListCell(r);
		}
	} /* r */
	#endif
	
	if (!cmIsEmptyList(&theValueHdr->valueList))
		display(f, "\n");

	return (WalkNextTOCValue);
}


/*--------------------------------------------------------------*
 | showValue - action routine for displaying a value in the TOC |
 *--------------------------------------------------------------*
 
 This is a debugging routine used as the "action" parameter to cmWalkThroughEntireTOC()
 calls to display selected info for a value in the TOC.  The "refCon" here is expected to
 be a file variable.
 
 Note, this "static" is intentionally left to default memory model under DOS since it is
 passed as a function pointer to cmWalkThroughEntireTOC().
*/
 
static TOCWalkReturns showValue(ContainerPtr container, TOCValuePtr theValue, CMRefCon refCon)
{
	ContainerPtr 	 unused = container;	
	FILE				 	 *f = (FILE *)refCon;
	CM_ULONG			 i, value4, valueLen;
	CM_USHORT			 mask, flags;
	CM_CHAR  			 *s, valueChars[5], flagsMeaning[256];
	CM_UCHAR			 *b;
	
	flags = theValue->flags;
#if !CMKEEP_CONTINUE_FLAG
	if (cmGetNextListCell(theValue))
		flags |= kCMContinued;
	else
		flags &= ~kCMContinued;
#endif
	
	if (flags == 0x0000U)
		*flagsMeaning = '\0';
	else {	
		strcpy(flagsMeaning, " (");
		
		for (i = 0, mask = 1; i < 16; ++i, mask <<= 1) {
			if ((flags & mask) == kCMImmediate) {
				strcat(flagsMeaning, "kCMImmediate/");
				continue;
			}
			if ((flags & mask) == kCMContinued) {
				strcat(flagsMeaning, "kCMContinued/");
				continue;
			}
			if ((flags & mask) == kCMDynValue) {
				strcat(flagsMeaning, "kCMDynValue/");
				continue;
			}
			if ((flags & mask) == kCMGlobalName) {
				strcat(flagsMeaning, "kCMGlobalName/");
				continue;
			}
		} /* for */
		
		strcpy(flagsMeaning + strlen(flagsMeaning) - 1, ")");
	}
	
	display(f, "                                    TOCValue\n"
						 "                                    0x%.8lX: theValueHdr     = 0x%.8lX\n"
						 "                                                flags           = 0x%.4X%s\n"
						 "                                                container       = 0x%.8lX (%s) [%d]\n"
						 "                                                logicalOffset   = 0x%.8lX = %ld\n",
						 theValue,
						 theValue->theValueHdr,
						 theValue->flags, flagsMeaning,
						 theValue->container, CONTAINERNAMEx(theValue->container), theValue->container->depth,
						 theValue->logicalOffset, theValue->logicalOffset);
							
	if (theValue->flags & kCMGlobalName)
		display(f, "                                                offset          = 0x%.8lX\n"
							 "                                                globalNameSymbol= 0x%.8lX\n"
							 "                                                \"%s\"\n",
							 theValue->value.globalName.offset, theValue->value.globalName.globalNameSymbol,
							 theValue->value.globalName.globalNameSymbol ? GetGlobalName(theValue->value.globalName.globalNameSymbol)
							 																						 : "????");
	else {
		valueLen = theValue->value.notImm.valueLen;
		value4   = theValue->value.notImm.value;
		
		if (flags & kCMImmediate) {
			for (i = 0, b = (CM_UCHAR *)&value4, s = valueChars; i < valueLen; ++i, ++b)
				*s++ = (CM_CHAR)((*b < (CM_UCHAR)' ' ||  *b > (CM_UCHAR)126) ? '.' : (*b));
			*s = '\0';
			
			if (valueLen == 1)
				value4 >>= 3 * CHAR_BIT;
			else if (valueLen == 2)
				value4 >>= 2 * CHAR_BIT;
			else  if (valueLen == 3)
				value4 >>= 1 * CHAR_BIT;
			
			display(f, "                                                value           = 0x%.*lX = '%s' = %ld\n"
								 "                                                length          = 0x%.8lX = %ld\n",
								 (int)(valueLen * 2), value4, valueChars, value4,
								 valueLen, valueLen);
		} else
			display(f, "                                                value           = 0x%.8lX = %ld\n"
								 "                                                length          = 0x%.8lX = %ld\n",
								 value4, value4,
								 valueLen, valueLen);
	}
	
	return (WalkNextTOCValue);
}


/*-------------------------------------------------------------------------*
 | CMDumpTOCStructures - dump all the TOC structures to the specified file |
 *-------------------------------------------------------------------------*
 
 This routine is used for internal debugging of the TOC data structures for the specified
 container.  It displays their current state to the (open) file associated with the
 specified file variable, f.
*/

void CM_FIXEDARGS CMDumpTOCStructures(CMContainer container, FILE CM_PTR *f)
{
	display(f, "\nTOC for type \"%s\" (%s) [%d]...\n\n", TYPENAME, CONTAINERNAME, ((ContainerPtr)container)->depth);
	cmWalkThroughEntireTOC((ContainerPtr)container, ((ContainerPtr)container)->toc,
													ALLOBJECTS, MAXUSERID, f,
													showObject, showProperty, showValueHdr, showValue);
	
	if (((ContainerPtr)container)->usingTargetTOC) {
		display(f, "\n\nPrivate TOC for type \"%s\" (%s)...\n\n", TYPENAME, CONTAINERNAME);
		cmWalkThroughEntireTOC((ContainerPtr)container, ((ContainerPtr)container)->privateTOC,
														ALLOBJECTS, MAXUSERID, f,
														showObject, showProperty, showValueHdr, showValue);
	}
	
	display(f, "\n");
}


#if TOC1_SUPPORT
/*-----------------------------------------------------------------------------*
 | dumpContainerTOCfmt1 - dump the format 1 TOC as it appears in the container |
 *-----------------------------------------------------------------------------*
 
 This routine is used for internal debugging of the format 1 TOC data as it appears in its
 final form in the container itself.  The entire TOC in the container, starting at 
 container offset tocOffset for tocSize bytes, is displayed to the (open) file associated
 with the file variable f.  It is formatted, but otherwise unchanged.  You can use this
 routine to see if the TOC was correctly written to its container.
 
 Note, the container's handler routines are used to do the positioning and reading.
*/

static void CM_NEAR dumpContainerTOCfmt1(CMconst_CMContainer container, FILE CM_PTR *f,
															 	 				 const CM_ULONG tocOffset, const CM_ULONG tocSize)
{	
	CM_ULONG		   objectID, propertyID, typeID;
	TOCValueBytes  value;
	CM_USHORT			 generation, flags;
	CM_UCHAR			 tocBuffer[TOCentrySize];
	CM_ULONG		   size      = (CM_ULONG)tocSize,
								 offset    = (CM_ULONG)tocOffset,
								 relOffset = 0;
	
	display(f, "\nTOC for type \"%s\" (%s)...\n\n", TYPENAME, CONTAINERNAME);
	
	display(f, "              | objectID |propertyID|  typeID  |  value   |  length  | gen  |flags |\n"
						 "              +----------+----------+----------+----------+----------+------+------+\n");
	
	/* Position to the start of the TOC...																								*/
	
	CMfseek(container, tocOffset, kCMSeekSet);
	
	while (size > 0) {											/* for each entry in the container's TOC...		*/
		if (CMfread(container, tocBuffer, sizeof(CM_UCHAR), TOCentrySize) != TOCentrySize) {
			display(f, "### Read error in cmDumpContainerTOC()\n");
			return;
		}
		
		ExtractTOC(tocBuffer, objectID, propertyID, typeID, value.notImm.value,
													value.notImm.valueLen, generation, flags);
				
		display(f, "%.8lX:%.4lX | %.8lX | %.8lX | %.8lX | %.8lX | %.8lX | %.4X | %.4X |\n",
										 offset, relOffset, objectID, propertyID, typeID, value.notImm.value,
										 value.notImm.valueLen, generation, flags);
		
		offset    += TOCentrySize;
		relOffset += TOCentrySize;

		size      -= TOCentrySize;						/* count down the size												*/
	} /* while */

	display(f, "%.8lX:%.4lX +----------+----------+----------+----------+----------+------+------+\n\n",
						  offset, relOffset);
}
#endif


/*------------------------------------------------------------------*
 | CMDumpContainerTOC - dump the TOC as it appears in the container |
 *------------------------------------------------------------------*
 
 This routine is used for internal debugging of the TOC data as it appears in its final
 form in the container itself.  The entire TOC in the container, starting at container
 offset tocOffset for tocSize bytes, is displayed to the (open) file associated with the
 file variable f.  It is formatted, but otherwise  unchanged.  You can use this routine to
 see if the TOC was correctly written to its container.
 
 Note, the container's handler routines are used to do the positioning and reading.
*/

void CM_FIXEDARGS CMDumpContainerTOC(CMconst_CMContainer container, FILE CM_PTR *f,
													  	 			 const CM_ULONG tocOffset, const CM_ULONG tocSize)
{	
	TOCentry			 tocEntry;
	CM_ULONG		   refsDataObjectID;
	CM_ULONG			 prevObjectID, prevPropertyID, prevTypeID;
	CM_ULONG		   offset, relOffset;
	void 					 *tocIOCtl;
	jmp_buf				 dumpEnv;
	
	USE_TOC_FORMAT_1_ALTERNATIVE1(dumpContainerTOCfmt1,(container, f, tocOffset, tocSize));
	
	/* Display the format 2 TOC.  Information is not repeated if it it wasn't repeated in */
	/* the actual TOC.																																		*/
	
	if (setjmp(dumpEnv)) 										/* set setjmp/longjmp environment vector			*/
		return;																/* ...just quit if there's a TOC input error	*/

	/* Position to the start of the TOC...																								*/
	
	CMfseek(container, tocOffset, kCMSeekSet);
	
	/* Create the TOC I/O control block and its buffer...																	*/
	
	tocIOCtl = cmStartTOCIO((ContainerPtr)container, ((ContainerPtr)container)->tocBufSize,
													(jmp_buf *)&dumpEnv, tocOffset, tocSize);
	if (tocIOCtl == NULL) return;
	
	prevObjectID = prevPropertyID = prevTypeID = 0x00000000UL;
	
	display(f, "\nTOC for type \"%s\" (%s)...\n\n", TYPENAME, CONTAINERNAME);
	
	display(f, "              | objectID |propertyID|  typeID  |generation|  value   |  length  |flags |  Ref ID  |\n"
						 "              +----------+----------+----------+----------+----------+----------+------+----------+\n");
	
	/* Read the entire TOC and display it in the following loop...												*/
	
	while (cmRead1TOCSegment(tocIOCtl, &tocEntry, &refsDataObjectID)) {
		offset = SESSION->currTOCoffset;
		relOffset = offset - tocOffset;
		display(f, "%.8lX:%.4lX ", offset, relOffset);
		
		if (tocEntry.objectID != prevObjectID) {
			prevObjectID 	 = tocEntry.objectID;
			prevPropertyID = tocEntry.propertyID;
			prevTypeID   	 = tocEntry.typeID;
			display(f, "| %.8lX | %.8lX | %.8lX | ", tocEntry.objectID, tocEntry.propertyID, tocEntry.typeID);
		} else if (tocEntry.propertyID != prevPropertyID) {
			prevPropertyID = tocEntry.propertyID;
			prevTypeID     = tocEntry.typeID;
			display(f, "|          | %.8lX | %.8lX | ", tocEntry.propertyID, tocEntry.typeID);
		} else if (tocEntry.typeID != prevTypeID) {
			prevTypeID = tocEntry.typeID;
			display(f, "|          |          | %.8lX | ", tocEntry.typeID);
		} else {
			display(f, "|          |          |          | ");
		}
		
		if (SESSION->gotExplicitGen) {
			display(f, "%.8lX | ", tocEntry.generation);
		} else
			display(f, "         | ");
		
		display(f, "%.8lX | %.8lX | %.4X | ", tocEntry.value.notImm.value, tocEntry.value.notImm.valueLen, tocEntry.flags);
		
		if (refsDataObjectID == 0x00000000UL)
			display(f, "         |\n");
		else
			display(f, "%.8lX |\n", refsDataObjectID);
	} /* while */
	
	offset = SESSION->currTOCoffset;
	relOffset = offset - tocOffset;
	display(f, "%.8lX:%.4lX +----------+----------+----------+----------+----------+----------+------+----------+\n\n",
							offset, relOffset);
	
	(void)cmEndTOCIO(tocIOCtl);
}
#endif 
														  CM_END_CFUNCTIONS
