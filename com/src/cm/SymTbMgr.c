/* @(#)Z 1.4 com/src/cm/SymTbMgr.c, odstorage, od96os2, odos29646d 96/11/15 15:27:15 (96/10/29 09:19:17) */
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
	File:		SymTbMgr.c

	Contains:	Container Manager Binary Tree Symbol Table Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1182319 The compare function in
									lookUpSymbol now compare name to symbol
									rather than symbol to symbol.
		 <1>	  2/3/94	EL		first checked in

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                         <<<    SymTbMgr.c    >>>                          |
 |                                                                           |
 |            Container Manager Binary Tree Symbol Table Routines            |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 11/18/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file contains all the generic symbol table routines.  They are in the form of 
 binary trees.  The three routines cmEnterSymbol(), cmLookupSymbol(), and cmForEachSymbol()
 are the low level generic binary tree manipulators which higher level "glue" routines
 use.
 
 All structs that are to be maintained as binary trees with this package must be of the
 form:
 
 				struct {
					SymbolLinks theLinks;
					...
				}...;
				
 In other words, a field (any name will do) of type SymbolLinks MUST be the first field
 of the structure.  The caller allocates all the struct symbol table entries.  This
 package enters them into a symbol table based on a tree root pointer and maintains the
 SymbolLinks.
 
 Being a generic package the links have to be at a know place in an otherwise arbitrary
 struct.  Hence the position requirement.
*/


#include <stddef.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __SYMMGR__
#include "SymTbMgr.h"      
#endif
#ifndef __SESSIONDATA__
#include "Session.h"          
#endif
#ifndef __HANDLERS__
#include "Handlers.h"
#endif
																	
																	CM_CFUNCTIONS

/* The following generates a segment directive for Mac only due to 32K Jump Table 			*/
/* Limitations.  If you don't know what I'm talking about don't worry about it.  The		*/
/* default is not to generate the pragma.  Theoritically unknown pragmas in ANSI won't	*/
/* choke compilers that don't recognize them.  Besides why would they be looked at if		*/
/* it's being conditionally skipped over anyway?  I know, famous last words!						*/

#if CM_MPW
#pragma segment SymbolTableMgr
#endif


/*---------------------------------------------------*
 | cmEnterSymbol - enter a symbol into a binary tree |
 *---------------------------------------------------*
 
 Enter the specified symbol into its own binary tree symbol table with the specified root.
 The function returns a pointer to the entry if entered.  If the entry is already there,
 the pointer to the dup entry is returned, dup is set to true, and no other action taken.
 
 It is assumed the space for the new symbol has already been allocated and its pointer
 passed as the symbol here.
 
 The binary tree is searched utilizing a comparison function provided by the caller.
 Compare is function that takes as arguments pointers to two symbols and returns -1 if the
 first symbol is "less than" the second, 1 if the first symbol is "greater than" the 
 second, and 0 if the tow symbols are equal.
*/
 
void *cmEnterSymbol(const void *symbol, void **root, Boolean *dup,
										int (*compare)(const void *, const void *))
{
	int		 				 comparison;
	SymbolLinksPtr p, prev;
	Boolean 			 lLeft = false;
	
	/* Look up symbol in the tree...																											*/
	
	if ((p = *(SymbolLinksPtr *)root) == NULL)
		prev = NULL;
	else
		do {
			prev = p;
			if ((comparison = (*compare)(p, symbol)) == 0) {
				*dup = true;															/* if entry already in the table...		*/
				return ((void *)p);												/* ...return ptr to it								*/
			} else if (comparison < 0) {
				p = p->rLink; lLeft = false;
			} else {
				p = p->lLink; lLeft = true;
			}
		} while (p);
	
	/* If we didn't find the symbol, enter it into the tree...														*/

	*dup = false;																	/* we know it's not a dup here					*/

	((SymbolLinksPtr)symbol)->lLink = ((SymbolLinksPtr)symbol)->rLink = NULL;	/* no links	*/
	
	if (!prev)																		/* hook new entry into tree							*/
		*root = (void *)symbol;
	else if (lLeft)
		prev->lLink = (SymbolLinksPtr)symbol;
	else
		prev->rLink = (SymbolLinksPtr)symbol;
	
	return ((void *)symbol);											/* return new entry to caller						*/
}


/*-----------------------------------------------------------------------------*
 | cmLookupSymbol - see if a symbol is already in its binary tree symbol table |
 *-----------------------------------------------------------------------------*
 
 Look up the specified symbol in its binary tree symbol table which starts at the
 specified root.  Return the pointer to it if found and NULL if not found.

 The binary tree is searched utilizing a comparison function provided by the caller. 
 Compare is function that takes as arguments pointers to two symbols and returns -1 if the
 first symbol is "less than" the second, 1 if the first symbol is "greater than" the 
 second, and 0 if the tow symbols are equal.
 
 It used to be that both arguments of compare are symbols, but that requires the
 temporary geneation of a symbol during lookup. So we change the comparision to
 a symbol and a name to improve the performance.
*/

void *cmLookupSymbol(const CM_UCHAR *name, const void *root,
									 	 int (*compare)(const void *, const CM_UCHAR *))
{
	SymbolLinksPtr p;
	int 				 	 comparison;

	p = (SymbolLinksPtr)root;
	while (p) {
		if ((comparison = (*compare)(p, name)) == 0)
			return ((void *)p);
		else if (comparison < 0)
			p = p->rLink;
		else
			p = p->lLink;
	}
	
	return (NULL);
}


/*-------------------------------------------------*
 | cmForEachSymbol - do some action on each symbol |
 *-------------------------------------------------*

 Do (call) the specified action for each entry in a binary tree symbol table. This
 routine recursively traverses the binary tree calling (*action)() on each entry visited.
 The pointer to the entry is passed to the action routine along with a "refCon" which the
 caller can use as a communication facility to convey additional info to the action
 routine.
 
 The search in the binary tree starts at the specified symbol location.  Generally this
 will be a root of a tree, but need not be.  Tree traversal is such that the symbols are
 visited in "ascending" order, i.e., whatever order that was used by the compare
 function passed to cmEnterSymbol().
*/
 
void cmForEachSymbol(const void *symbol, CMRefCon refCon,
							   		 void (*action)(void *symbol, CMRefCon refCon))
{
	void *lLink, *rLink;
	
	if (symbol) {																/* if we got a symbol...									*/
		lLink = ((SymbolLinksPtr)symbol)->lLink;	/* ...hold on to links in case action does*/
		rLink = ((SymbolLinksPtr)symbol)->rLink;	/*    something "radical" with the entries*/
	
		cmForEachSymbol(lLink, refCon, action);		/* ...follow the left link down						*/
		(*action)((void *)symbol, (void *)refCon);/* ...perform action at a leaf						*/
		cmForEachSymbol(rLink, refCon, action);		/* ...do same for the right links					*/
	}
}


/*---------------------------------------------------------------------------------------*
 | freeSymbols - free all symbols in a binary tree symbol tbl (guts of cmFreeAllSymbols) |
 *---------------------------------------------------------------------------------------*
 
 This is used only by cmFreeAllSymbols() to walk the symbol table binary tree to free all
 the symbols in it.  It is functionally equivalent to cmForEachSymbol() above. Indeed we
 could have used (and once upon a time, did) cmForEachSymbol() if it we not for the refCon
 usage!
*/

static void CM_NEAR freeSymbols(const void *symbol, SessionGlobalDataPtr sessionData)
{
	void *lLink, *rLink;
	
	if (symbol) {																/* if we got a symbol...									*/
		lLink = ((SymbolLinksPtr)symbol)->lLink;	/* get the two links...										*/
		rLink = ((SymbolLinksPtr)symbol)->rLink;
		
		freeSymbols(lLink, sessionData);					/* ...follow the left link down						*/
		SessionFree(symbol);											/* ...free a symbol												*/
		freeSymbols(rLink, sessionData);					/* ...do same for the right links					*/
	}
}


/*-------------------------------------------------------------------------*
 | cmFreeAllSymbols - remove all the symbols in a binary tree symbol table |
 *-------------------------------------------------------------------------*
 
 This routine takes a root of a binary tree symbol table and deletes ALL the entries in
 the table.  The root pointer is returned as NULL.
 
 Note, that SymbolTableMgr routines are low level routines used in a number of contexts.
 Here we need to uutilize the container memory deallocator handler.  Because we don't
 know the context, which may be global, the session global data pointer is passed and we
 access the handler through that.
*/

void cmFreeAllSymbols(void **root, SessionGlobalDataPtr sessionData)
{
	freeSymbols(*root, sessionData);
	*root = NULL;
}
														  
															CM_END_CFUNCTIONS
