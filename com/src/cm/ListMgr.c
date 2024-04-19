/* @(#)Z 1.4 com/src/cm/ListMgr.c, odstorage, od96os2, odos29646d 96/11/15 15:27:13 (96/10/29 09:18:58) */
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
	File:		ListMgr.c

	Contains:	Container Manager Doubly Linked Lists Routines

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.
		 <2>	  2/3/94	EL		Bento File names are now eight chars or
													less.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                             <<< ListMgr.c >>>                             |
 |                                                                           |
 |               Container Manager Doubly Linked Lists Routines              |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 11/26/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 This file contains all the generic doubly linked list routines.  The routines are the
 low level generic doubly linked list manipulators which the higher level "glue" routines
 use.
 
 All structs list cells that are to be maintained as doubly linked lists with this package
 must be of the form:
 
 				struct {
					ListLinksPtr theLinks;
					...
				}...;
				
 In other words, a field (any name will do) of type ListLinksPtr MUST be the first field
 of the struct.  The caller allocates all the struct list cells.  This package enters
 them into a list based whose head and tail are pointed to by a header which takes the
 following form:
 
 				struct {
					ListHdr theListHeader;
					...
				}...;
 
 This is similar to the list entries themselves.  Here a ListHdr is the first field of
 some structure.
 
 Being a generic package the links and the header have to be at a know place in otherwise
 arbitrary structs.  Hence the position requirements.
*/
 
 
#include <stddef.h>
#include <stdio.h>

#ifndef __CMTYPES__
#include "CMTypes.h"
#endif
#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif
#ifndef __LISTMGR__
#include "ListMgr.h"
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
#pragma segment ListMgr
#endif


/*-------------------------------------------------------------------*
 | cmInitList - init a liast header with NULL head and tail pointers |
 *-------------------------------------------------------------------*
 
 This routine takes a list header and initializes the head and tail pointers to NULL.  All
 empty lists are assumed to have NULL head and tail pointers.  The function returns the
 input header pointer as its result.
*/

void *cmInitList(const void *theList)
{
	((ListHdrPtr)theList)->head = ((ListHdrPtr)theList)->tail = NULL;
	((ListHdrPtr)theList)->nbrOfCells = 0;
	return ((void *)theList);
}


/*-----------------------------------------------------------------*
 | cmInsertBeforeListCell - insert a list cell BEFORE another cell |
 *-----------------------------------------------------------------*
 
 Given a pointer to a list header (theList), this routine inserts a new cell (theCell)
 before another cell already on the list (beforeThisCell).  The function returns the
 input inserted cell pointer as its result.
 
 If beforeThisCell is NULL this function inserts theCell at the beginning of the list.
 
 If theCell is NULL, nothing is done and NULL is returned.
*/

void *cmInsertBeforeListCell(const void *theList, const void *theCell, const void *beforeThisCell)
{
	if (theCell == NULL) return (NULL);															/* safety 						*/
	
	if (beforeThisCell == NULL) {																		/* insert at beginning*/
		((ListLinksPtr)theCell)->prev = NULL;
		((ListLinksPtr)theCell)->next = ((ListHdrPtr)theList)->head;
		
		if (((ListHdrPtr)theList)->head)
			((ListHdrPtr)theList)->head->prev = (ListLinksPtr)theCell;
		((ListHdrPtr)theList)->head = (ListLinksPtr)theCell;
		
		if (((ListHdrPtr)theList)->tail == NULL)
			((ListHdrPtr)theList)->tail = (ListLinksPtr)theCell;
	} else {																												/* insert before 			*/
		((ListLinksPtr)theCell)->prev = ((ListLinksPtr)beforeThisCell)->prev;
		((ListLinksPtr)theCell)->next = (ListLinksPtr)beforeThisCell;
		
		if (((ListLinksPtr)beforeThisCell)->prev)
			((ListLinksPtr)beforeThisCell)->prev->next = (ListLinksPtr)theCell;
		else																													/* new head 					*/
			((ListHdrPtr)theList)->head = (ListLinksPtr)theCell;
		
		((ListLinksPtr)beforeThisCell)->prev = (ListLinksPtr)theCell;
	}
	
	++((ListHdrPtr)theList)->nbrOfCells;														/* count cell					*/
	
	return ((void *)theCell);
}


/*---------------------------------------------------------------*
 | cmInsertAfterListCell - insert a list cell AFTER another cell |
 *---------------------------------------------------------------*
 
 Given a pointer to a list header (theList), this routine inserts a new cell (theCell)
 after another cell already on the list (beforeThisCell).  The function returns the
 input inserted cell pointer as its result.
 
 If afterThisCell is NULL this function appends theCell to the end of the list.
 
 If theCell is NULL, nothing is done and NULL is returned.
*/

void *cmInsertAfterListCell(const void *theList, const void *theCell, const void *afterThisCell)
{
	if (theCell == NULL) return (NULL);															/* safety 						*/
	
	if (afterThisCell == NULL) {																		/* append							*/
		((ListLinksPtr)theCell)->next = NULL;
		((ListLinksPtr)theCell)->prev = ((ListHdrPtr)theList)->tail;

		if (((ListHdrPtr)theList)->tail == NULL)
			((ListHdrPtr)theList)->head = (ListLinksPtr)theCell;
		else
			((ListHdrPtr)theList)->tail->next = (ListLinksPtr)theCell;
		((ListHdrPtr)theList)->tail = (ListLinksPtr)theCell;
	} else {																												/* insert after				*/
		((ListLinksPtr)theCell)->prev = (ListLinksPtr)afterThisCell;
		((ListLinksPtr)theCell)->next = ((ListLinksPtr)afterThisCell)->next;
		
		if (((ListLinksPtr)afterThisCell)->next)
			((ListLinksPtr)afterThisCell)->next->prev = (ListLinksPtr)theCell;
		else																													/* new tail 					*/
			((ListHdrPtr)theList)->tail = (ListLinksPtr)theCell;
		
		((ListLinksPtr)afterThisCell)->next = (ListLinksPtr)theCell;
	}
	
	++((ListHdrPtr)theList)->nbrOfCells;														/* count cell					*/
	
	return ((void *)theCell);
}


/*-------------------------------------------------------*
 | cmAppendListCell - append a cell to the end of a list |
 *-------------------------------------------------------*
 
 This function is the same as a cmInsertAfterListCell(theList, theCell, NULL), i.e., 
 theCell is appended to the end of the list.  The function returns the input inserted
 cell pointer as its result.
 
 If theCell is NULL, nothing is done and NULL is returned.
*/

void *cmAppendListCell(const void *theList, const void *theCell)
{
	if (theCell == NULL) return (NULL);															/* safety						 */
	
	if (((ListHdrPtr)theList)->tail == NULL)
		((ListHdrPtr)theList)->head = (ListLinksPtr)theCell;
	else
		((ListHdrPtr)theList)->tail->next = (ListLinksPtr)theCell;
	((ListLinksPtr)theCell)->prev = ((ListHdrPtr)theList)->tail;
	
	((ListLinksPtr)theCell)->next = NULL;
	((ListHdrPtr)theList)->tail = (ListLinksPtr)theCell;
	
	++((ListHdrPtr)theList)->nbrOfCells;														/* count cell					*/
	
	return ((void *)theCell);
}


/*---------------------------------------*
 | cmDeleteListCell - delete a list cell |
 *---------------------------------------*
 
 This function removes the specified cell (theCell) from a list whose header is pointed
 to by theList.  It is up to the caller to free the memory occupied by the cell.  Here
 it is simply "jump out" of the list link structure.  The input cell pointer (theCell) is
 returned as the function result.

 If theCell is NULL, nothing is done and NULL is returned.
*/

void *cmDeleteListCell(const void *theList, const void *theCell)
{
	if (theCell == NULL) return (NULL);														/* safety 							*/
	
	if (((ListLinksPtr)theCell)->prev)
		((ListLinksPtr)theCell)->prev->next = ((ListLinksPtr)theCell)->next;
	else 																													/* delete head 					*/
		((ListHdrPtr)theList)->head = ((ListLinksPtr)theCell)->next;
	
	if (((ListLinksPtr)theCell)->next)
		((ListLinksPtr)theCell)->next->prev = ((ListLinksPtr)theCell)->prev;
	else 																													/* delete tail 					*/
		((ListHdrPtr)theList)->tail = ((ListLinksPtr)theCell)->prev;
	
	--((ListHdrPtr)theList)->nbrOfCells;													/* decrement count			*/
	
	return ((void *)theCell);
}


#if !LISTMACROS
/*-------------------------------------------------*
 | cmGetNextListCell - get the next cell on a list |
 *-------------------------------------------------*

 Given a pointer to a list cell, this function returns the pointer to the next cell on
 the list or NULL if there is no next cell.
 
 NULL is also returned if the input cell pointer is NULL.
*/

void *cmGetNextListCell(void *currCell)
{
	if (currCell == NULL) return(NULL);														/* safety 							*/
	return (((ListLinksPtr)currCell)->next);
}
#endif


#if !LISTMACROS
/*-----------------------------------------------------*
 | cmGetPrevListCell - get the previous cell on a list |
 *-----------------------------------------------------*

 Given a pointer to a list cell, this function returns the pointer to the previous cell
 on the list or NULL if there is no previous cell.
 
 NULL is also returned if the input cell pointer is NULL.
*/

void *cmGetPrevListCell(void *currCell)
{
	if (currCell == NULL) return(NULL);														/* safety 							*/
	return (((ListLinksPtr)currCell)->prev);
}
#endif


#if !LISTMACROS
/*---------------------------------------------------------*
 | cmCountListCells - return the number of cells in a list |
 *---------------------------------------------------------*
 
 This function can be used to determine the number of cells in a list.  0 is returned if
 the list is currently empty. It is assumed that the list has been previously initialized
 by cmInitList().
*/

CM_ULONG cmCountListCells(const void *theList)
{
	return (((ListHdrPtr)theList)->nbrOfCells);
}
#endif


#if !LISTMACROS
/*----------------------------------------------*
 | cmIsEmptyList - determine if a list is empty |
 *----------------------------------------------*
 
 This function returns true if the specified list is empty (i.e., contains no cells) and
 false otherwise (i.e., contains cells).
*/

CMBoolean cmIsEmptyList(const void *theList)
{
	return (((ListHdrPtr)theList)->nbrOfCells == 0);
}
#endif


#if !LISTMACROS
/*----------------------------------------------------------*
 | cmGetListhead - return the pointer to the head of a list |
 *----------------------------------------------------------*/

void *cmGetListHead(const void *theList)
{
	return (((ListHdrPtr)theList)->head);
}
#endif


#if !LISTMACROS
/*----------------------------------------------------------*
 | cmGetListTail - return the pointer to the tail of a list |
 *----------------------------------------------------------*/

void *cmGetListTail(const void *theList)
{
	return (((ListHdrPtr)theList)->tail);
}
#endif


#if !LISTMACROS
/*----------------------------------------------------------*
 | cmNullListLinks - force the links in a list cell to NULL |
 *----------------------------------------------------------*

 This is generally done as a safety measure after a cell is allocated.  If the cell finds
 its way into a linked list then most list walkers will be happy with NULL list links if
 they see them.  "Bad" cells like these could arise from error conditions which may be
 seen during a cleanup.
*/

void *cmNullListLinks(void *theCell)
{
	if (currCell == NULL) return(NULL);														/* safety 							*/
	((ListLinksPtr)theCell)->prev = ((ListLinksPtr)theCell)->next = NULL;
	return (theCell);
}
#endif


/*------------------------------------------------*
 | cmGetNthListCell - get the N'th cell on a list |
 *------------------------------------------------*
 
 This function returns a pointer to the N'th cell (counting from 1) on a list whose
 header is pointed to by theList.  NULL is returned if there is no N'th list item.
*/

void *cmGetNthListCell(const void *theList, const CM_ULONG n)
{
	CM_ULONG			i;
	ListLinksPtr 	p;
	
	if (n == 0 || n > ((ListHdrPtr)theList)->nbrOfCells) 				/* must have valid "n"		*/
		return (NULL);
	
	p = (ListLinksPtr)((ListHdrPtr)theList)->head;							/* hunt n'th cell down		*/
	for (i = 2; i <= n && p; ++i) p = p->next;
	return (p);
}


/*-------------------------------------------------------------------------------*
 | cmInsertBeforeNthListCell - insert a list cell BEFORE the N'th cell on a list |
 *-------------------------------------------------------------------------------*
 
 This function inserts the specified cell (theCell) before the N'th cell (counting from
 1) on the list whose header is pointed to by theList.  The function returns the input
 inserted cell pointer as its result.
 
 Nothing is inserted and NULL is returned if if there is no N'th list item or theCell is
 NULL.
*/

void *cmInsertBeforeNthListCell(const void *theList, const void *theCell, const CM_ULONG n)
{
	void *beforeThisCell;
	
	beforeThisCell = cmGetNthListCell(theList, n);							/*  find n'th cell				*/
	if (beforeThisCell == NULL) return (NULL);									/* return NULL if no n'th	*/
	
	return (cmInsertBeforeListCell(theList, theCell, beforeThisCell)); /* insert cell			*/
}


/*-----------------------------------------------------------------------------*
 | cmInsertAfterNthListCell - insert a list cell AFTER the N'th cell on a list |
 *-----------------------------------------------------------------------------*

 This function inserts the specified cell (theCell) after the N'th cell (counting from
 1) on the list whose header is pointed to by theList.  The function returns the input
 inserted cell pointer as its result.
 
 Nothing is inserted and NULL is returned if if there is no N'th list item or theCell is
 NULL.
*/

void *cmInsertAfterNthListCell(const void *theList, const void *theCell, const CM_ULONG n)
{
	void *afterThisCell;
	
	afterThisCell = cmGetNthListCell(theList, n);								/* find n'th cell					*/
	if (afterThisCell == NULL) return (NULL);										/* return NULL if no n'th	*/

	return (cmInsertAfterListCell(theList, theCell, afterThisCell));	/* insert cell			*/
}


/*---------------------------------------------------------------------------*
 | cmInsertNthListCell - insert a list cell into the N'th position on a list |
 *---------------------------------------------------------------------------*
 
 This function makes the specified cell (theCell) the N'th cell (counting from 1) on the
 list whose header is pointed to by theList.  The function returns the input inserted
 cell pointer as its result.
 
 Nothing is inserted and NULL is returned or if N is specified as 0 or 1 greater than
 the total number of cells currently on the list.  NULL is also returned if theCell is
 NULL.
 
 If N is specified a 1 greater than the total number of cells currently on the list then
 the new cell is APPENDED to the end of the list.  If N is anything less, the new cell
 is inserted BEFORE the old N'th cell.  Thus the new cell becomes the N'th cell.
*/

void *cmInsertNthListCell(const void *theList, const void *theCell, const CM_ULONG n)
{
	if (n == 0 || n > ((ListHdrPtr)theList)->nbrOfCells + 1)		/* must have valid "n"		*/
		return (NULL);
	
	if (n == ((ListHdrPtr)theList)->nbrOfCells + 1)							/* if we're appending...	*/
		return (cmAppendListCell(theList, theCell));							/* ...do it and exit			*/
	
	return (cmInsertBeforeNthListCell(theList, theCell, n));		/* insert new N'th cell		*/
}


/*-------------------------------------------------------------*
 | cmGetCellPosition - find the position of a cell in its list |
 *-------------------------------------------------------------*
 
 This function returns the position index, 1 to N, of the cell (theCell) in the list whose
 header is pointed to by theList.  The function returns 0 if theCell cannot be found in 
 theList.
*/

CM_ULONG cmGetCellPosition(const void *theList, const void *theCell)
{
	CM_ULONG		  i = 0;
	ListLinksPtr  p = (ListLinksPtr)((ListHdrPtr)theList)->head;

	while (p) {																									/* loop through all cells	*/
		++i;																											/* count the cell					*/
		if (p == (ListLinksPtr)theCell) return (i);								/* return count if found	*/
		p = p->next;																							/* keep looking						*/
	}
	
	return (0);																									/* didn't find it					*/
}


/*------------------------------------------------------*
 | cmForEachListCell - do some action on each list cell |
 *------------------------------------------------------*
 
 Do (call) the specified action for each cell on the specified list whose header is 
 pointed to by theList. This routine calls (*action)() on each cell along with a "refCon"
 which the caller can use as a communication facility to convey additional info to the
 action routine.  The pointer to the cell is passed to the action routine.
*/

void cmForEachListCell(const void *theList, CMRefCon refCon,
										 	 void (*action)(void *cell, CMRefCon refCon))
{
	ListLinksPtr next, p = (ListLinksPtr)((ListHdrPtr)theList)->head;
	
	while (p) {																									/* loop through all cells	*/
		next = p->next;																						/* cell could be deleted	*/
		(*action)(p, refCon);
		p = next;
	}
}


/*-------------------------------------------------------*
 | cmFreeAllListCells - remove all the cells from a list |
 *-------------------------------------------------------*
 
 This routine removes (i.e., free()s) all the cells from the specified list.  The list
 header is reinitialized.  Because it uses the memory deallocator it need the global data
 session pointer.
*/

void cmFreeAllListCells(const void *theList, SessionGlobalDataPtr sessionData)
{
	ListLinksPtr next, p = (ListLinksPtr)((ListHdrPtr)theList)->head;
	
	while (p) {																									/* loop through all cells	*/
		next = p->next;																						/* cell is being deleted	*/
		SessionFree(p);
		p = next;
	}
	
	cmInitList(theList);
}
														  
															CM_END_CFUNCTIONS
