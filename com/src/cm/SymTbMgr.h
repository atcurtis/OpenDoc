/* @(#)Z 1.4 com/src/cm/SymTbMgr.h, odstorage, od96os2, odos29646d 96/11/15 15:27:15 (96/10/29 09:19:22) */
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
	File:		SymTbMgr.h

	Contains:	Container Manager Binary Tree Symbol Interfaces

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
 |                         <<<    SymTbMgr.h    >>>                          |
 |                                                                           |
 |              Container Manager Binary Tree Symbol Interfaces              |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 11/18/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*
 
 The SymbolTableMgr contains all the generic symbol table routines.  They are in the form
 of binary trees.  The three routines cmEnterSymbol(), cmLookupSymbol(), and
 cmForEachSymbol() are the low level generic binary tree manipulators which higher level
 "glue" routines use.
 
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
 
 
#ifndef __SYMMGR__
#define __SYMMGR__


#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif

struct SessionGlobalData;
																	CM_CFUNCTIONS


struct SymbolLinks {														/* must be the first field in any symbol*/
	struct SymbolLinks *lLink, *rLink;						/*		left/right binary tree links			*/
};
typedef struct SymbolLinks SymbolLinks, *SymbolLinksPtr;


void *cmEnterSymbol(const void *symbol, void **root, Boolean *dup,
										int (*compare)(const void *, const void *));
	/*
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


void *cmLookupSymbol(const CM_UCHAR *name, const void *root,
									 	 int (*compare)(const void *, const CM_UCHAR *));
 /*
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


void cmForEachSymbol(const void *symbol, CMRefCon refCon,
									 	 void (*action)(void *symbol, CMRefCon refCon));
	/*
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
	
	
typedef void (*SymbolAction)(void *, CMRefCon);
	/*
	The cmForEachSymbol() action routine is defined as the generic action routine to call
	for a table consisting of symbols of a particular type.  Calling cmForEachSymbol() will
	thus require casting the callers action routine to the generic type.  This typedef is
	used for that purpose.  It must be here under the exter "C" C++ linkage specification
	so that the cast, like the cmForEachSymbol() proptotype uses C calling conventions.
	*/ 
	
	
void cmFreeAllSymbols(void **root, struct SessionGlobalData *sessionData);
	/*
  This routine takes a root of a binary tree symbol table and deletes ALL the entries in
  the table.  The root pointer is returned as NULL.
 
	Note, that SymbolTableMgr routines are low level routines used in a number of contexts.
	Here we need to uutilize the container memory deallocator handler.  Because we don't
	know the context, which may be global, the session global data pointer is passed and we
	access the handler through that.
	*/
	
	
														  CM_END_CFUNCTIONS
#endif
