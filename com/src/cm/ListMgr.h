/* @(#)Z 1.4 com/src/cm/ListMgr.h, odstorage, od96os2, odos29646d 96/11/15 15:27:13 (96/10/29 09:19:02) */
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
	File:		ListMgr.h

	Contains:	Container Manager Doubly Linked Lists Interfaces

	Written by:	Ira L. Ruben

	Owned by:	Ed Lai

	Copyright:	© 1991-1994 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	 8/26/94	EL		#1181622 Ownership update.

	To Do:
*/

/*---------------------------------------------------------------------------*
 |                                                                           |
 |                             <<< ListMgr.h >>>                             |
 |                                                                           |
 |              Container Manager Doubly Linked Lists Interfaces             |
 |                                                                           |
 |                               Ira L. Ruben                                |
 |                                 11/26/91                                  |
 |                                                                           |
 |                  Copyright Apple Computer, Inc. 1991-1994                 |
 |                           All rights reserved.                            |
 |                                                                           |
 *---------------------------------------------------------------------------*

 The ListMgr package contains all the generic doubly linked list routines. The routines are
 the low level generic doubly linked list manipulators which the higher level "glue"
 routines use.
 
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
 some structure.  Note, it is NOT necessary to make the ListHdr the first entry of a
 larger structure if you pass the pointer to ListHdr pointer explicitly as the list
 header pointer to any of the list routines defined here.
 
 Being a generic package the links and the header have to be at a know place in otherwise
 arbitrary structs.  Hence the position requirements.
*/


#ifndef __LISTMGR__
#define __LISTMGR__

#ifndef __CM_API_TYPES__
#include "CMAPITyp.h"
#endif

struct SessionGlobalData;

																	CM_CFUNCTIONS

struct ListLinks {													/* must be the first field in any list entry */
	struct ListLinks *next, *prev;						/*		next/previous list links							 */
};
typedef struct ListLinks ListLinks, *ListLinksPtr;

struct ListHdr {														/* all list headers must look like this			*/
	ListLinksPtr 	head, tail;									/* 		head/tail of the list									*/
	CM_ULONG		nbrOfCells;									/*		number of cells on the list						*/
};																					/* struct can be 1st thin in a larger struct*/
typedef struct ListHdr ListHdr, *ListHdrPtr;

#ifndef LISTMACROS													
#define LISTMACROS 1												/* 0 for less optimum function calls				*/
#endif


void *cmInitList(const void *theList);
	/*
	This routine takes a list header and initializes the head and tail pointers to NULL.  All
	empty lists are assumed to have NULL head and tail pointers.  The function returns the
	input header pointer as its result.
	*/
	
	
void *cmInsertBeforeListCell(const void *theList, const void *theCell, const void *beforeThisCell);
	/*
	Given a pointer to a list header (theList), this routine inserts a new cell (theCell)
	before another cell already on the list (beforeThisCell).  The function returns the
	input inserted cell pointer as its result.
	
	If beforeThisCell is NULL this function inserts theCell at the beginning of the list.
	
	If theCell is NULL, nothing is done and NULL is returned.
	*/
	
	
void *cmInsertAfterListCell(const void *theList, const void *theCell, const void *afterThisCell);
	/*
	Given a pointer to a list header (theList), this routine inserts a new cell (theCell)
	after another cell already on the list (beforeThisCell).  The function returns the
	input inserted cell pointer as its result.
	
	If afterThisCell is NULL this function appends theCell to the end of the list.
	
	If theCell is NULL, nothing is done and NULL is returned.
	*/
	
	
void *cmAppendListCell(const void *theList, const void *theCell);
	/*
	This function is the same as a cmInsertAfterListCell(theList, theCell, NULL), i.e., 
	theCell is appended to the end of the list.  The function returns the input inserted
	cell pointer as its result.
	
	If theCell is NULL, nothing is done and NULL is returned.
	*/
	
	
void *cmDeleteListCell(const void *theList, const void *theCell);
	/*
	This function removes the specified cell (theCell) from a list whose header is pointed
	to by theList.  It is up to the caller to free the memory occupied by the cell.  Here
  it is simply "jump out" of the list link structure.  The input cell pointer (theCell) is
  returned as the function result.
 
	If theCell is NULL, nothing is done and NULL is returned.
	*/
	
	
#if LISTMACROS
#define cmGetNextListCell(currCell) ((void *)(((ListLinksPtr)(currCell))->next))
#else
void *cmGetNextListCell(void *currCell);
	/*
  Given a pointer to a list cell, this function returns the pointer to the next cell on
  the list or NULL if there is no next cell.
 
  NULL is also returned if the input cell pointer is NULL.
	*/
#endif

	
#if LISTMACROS
#define cmGetPrevListCell(currCell) ((void *)(((ListLinksPtr)(currCell))->prev))
#else
void *cmGetPrevListCell(void *currCell);
	/*
  Given a pointer to a list cell, this function returns the pointer to the previous cell
  on the list or NULL if there is no previous cell.
  
  NULL is also returned if the input cell pointer is NULL.
	*/
#endif
	
	
#if LISTMACROS
#define cmCountListCells(theList) ((long)(((ListHdrPtr)(theList))->nbrOfCells))
#else
CM_ULONG cmCountListCells(const void *theList);
	/*
  This function can be used to determine the number of cells in a list.  0 is returned if
  the list is currently empty. It is assumed that the list has been previously initialized
  by cmInitList().
	*/
#endif
	
	
#if LISTMACROS
#define cmIsEmptyList(theList) (((ListHdrPtr)(theList))->nbrOfCells == 0)
#else
CMBoolean cmIsEmptyList(const void *theList);
	/*
  This function returns true if the specified list is empty (i.e., contains no cells) and
  false otherwise (i.e., contains cells).
	*/
#endif


#if LISTMACROS
#define cmGetListHead(theList) ((void *)(((ListHdrPtr)(theList))->head))
#else
void *cmGetListHead(const void *theList);
	/*
	Return the pointer to the head of a list.
	*/
#endif
	

#if LISTMACROS
#define cmGetListTail(theList) ((void *)(((ListHdrPtr)(theList))->tail))
#else
void *cmGetListTail(const void *theList);
	/*
	Return the pointer to the tail of a list.
	*/
#endif


#if LISTMACROS
#define cmNullListLinks(theCell) (((ListLinksPtr)(theCell))->prev = ((ListLinksPtr)(theCell))->next = NULL, (void *)(theCell))
#else
void *cmNullListLinks(void *theCell);
	/*
	Force the links in a list cell to NULL.  This is generally done as a safety measure
	after a cell is allocated.  If the cell finds its way into a linked list then most list
	walkers will be happy with NULL list links if they see them.  "Bad" cells like these
	could arise from error conditions which may be seen during a cleanup.
	*/
#endif
	
	
void *cmGetNthListCell(const void *theList, const CM_ULONG n);
	/*
  This function returns a pointer to the N'th cell (counting from 1) on a list whose
  header is pointed to by theList.  NULL is returned if there is no N'th list item.
	*/
	
	
void *cmInsertBeforeNthListCell(const void *theList, const void *theCell, const CM_ULONG n);
	/*
	This function inserts the specified cell (theCell) before the N'th cell (counting from
	1) on the list whose header is pointed to by theList.  The function returns the input
	inserted cell pointer as its result.
	
	Nothing is inserted and NULL is returned if if there is no N'th list item or theCell is
	NULL.
	*/
	
	
void *cmInsertAfterNthListCell(const void *theList, const void *theCell, const CM_ULONG n);	
	/*
	This function inserts the specified cell (theCell) after the N'th cell (counting from
	1) on the list whose header is pointed to by theList.  The function returns the input
	inserted cell pointer as its result.
	
	Nothing is inserted and NULL is returned if if there is no N'th list item or theCell is
	NULL.
	*/


void *cmInsertNthListCell(const void *theList, const void *theCell, const CM_ULONG n);
	/*
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
	
	
CM_ULONG cmGetCellPosition(const void *theList, const void *theCell);
	/*
	This function returns the position index, 1 to N, of the cell (theCell) in the list whose
	header is pointed to by theList.  The function returns 0 if theCell cannot be found in 
	theList.
	*/
	

void cmForEachListCell(const void *theList, CMRefCon refCon,
										 	 void (*action)(void *cell, CMRefCon refCon));
	/*
	Do (call) the specified action for each cell on the specified list whose header is 
	pointed to by theList. This routine calls (*action)() on each cell along with a "refCon"
	which the caller can use as a communication facility to convey additional info to the
	action routine.  The pointer to the cell is passed to the action routine.
	*/
	
	
void cmFreeAllListCells(const void *theList, struct SessionGlobalData *sessionData);
	/*
	This routine removes (i.e., free()s) all the cells from the specified list.  The list
	header is reinitialized.  Because it uses the memory deallocator it need the global data
	session pointer.
	*/
	

														  CM_END_CFUNCTIONS
#endif
