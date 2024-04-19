//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES:   Node
//		NodeTraverser
//
//   ORIGINS: 82,27
//
//
//   (C) COPYRIGHT International Business Machines Corp. 1995,1996
//   All Rights Reserved
//   Licensed Materials - Property of IBM
//   US Government Users Restricted Rights - Use, duplication or
//   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//   	
//   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
//   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
//   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
//   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
//   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
//   OR PERFORMANCE OF THIS SOFTWARE.
//
//====END_GENERATED_PROLOG========================================
//
// @(#) 1.4 com/src/utils/include/Node.h, odutils, od96os2, odos29646d 7/15/96 18:00:47 [ 11/15/96 15:29:33 ]
/*
	File:		Node.h

	Contains:	Tree node class

	Owned by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
*/

#ifndef _NODE_
#define _NODE_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _LINKLIST_
#include "LinkList.h"
#endif

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class Node;
class NodeTraverser;

//=====================================================================================
// Classes used by this interface
//=====================================================================================

class Link;
class LinkedList;

//=====================================================================================
// Class Node
//=====================================================================================

class  Node : private Link, private LinkedList
{
	public:
		Node();
		~Node();

		ODULong Size();
		
		Node* GetParent();
		Node* GetFirstChild();
		Node* GetLastChild();
		Node* GetNextSibling();
		Node* GetPreviousSibling();
			
		void SetParent(Node* parent);
		
		void AddChildFirst(Node* node);
		void AddChildLast(Node* node);
		void AddChildBefore(Node& existing, Node* node);
		void AddChildAfter(Node& existing, Node* node);
		void RemoveChild(Node& node);
		Node* GetChildAfter(Node* node);
		Node* GetChildBefore(Node* node);

		Node* FirstTopDown();
		Node* NextTopDown(ODSiblingOrder siblingOrder);
		Node* GetNextAunt(ODSiblingOrder siblingOrder);
		
#ifndef OD_BUG  // Method doesn't work, and no one in DR4 uses it.
		Node* FirstBottomUp(ODSiblingOrder siblingOrder);
		Node* NextBottomUp(ODSiblingOrder siblingOrder);
#endif

	private:
		Node*		fParent;
};

//=====================================================================================
// Class NodeTraverser
//=====================================================================================


class NodeTraverser 
{
public:

	NodeTraverser(Node* root, 
				  ODTraversalType traversalType, 
				  ODSiblingOrder siblingOrder);
	~NodeTraverser();

	Node*				First();
	Node*				Next();
	void				SkipChildren();
	ODBoolean			IsNotComplete();

protected:	

	Node* FirstTopDown(Node* node);
	Node* NextTopDown(Node* node, ODSiblingOrder siblingOrder);
	Node* GetNextAunt(Node* node, ODSiblingOrder siblingOrder);
	Node* FirstBottomUp(Node* node, ODSiblingOrder siblingOrder);
	Node* NextBottomUp(Node* node, ODSiblingOrder siblingOrder);
	
private:
  	Node*			 fRoot;
 	Node*			 fCurrent;
 	ODTraversalType fTraversalType;
 	ODSiblingOrder  fSiblingOrder;
};

#endif // _NODE_
