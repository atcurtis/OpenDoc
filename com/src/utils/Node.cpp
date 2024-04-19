//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odutils
//
//   CLASSES: none
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
// @(#) 1.7 com/src/utils/Node.cpp, odutils, od96os2, odos29646d 7/15/96 17:58:39 [ 11/15/96 15:28:45 ]
/*
	File:		Node.cpp

	Contains:	Implementation of Node class

	Owned by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	To Do:
		Optimization: Many methods could be inlined for speed & code size improvements.
*/

#if defined(_PLATFORM_OS2_) || defined(_PLATFORM_WIN32_)
#include <ODPagtun.h>
#endif

#ifndef _NODE_
#include "Node.h"
#endif

#ifndef _ODEXCEPT_
#include "ODExcept.h"
#endif

#ifndef _ODDEBUG_
#include "ODDebug.h"
#endif


#pragma segment ODNode

//======================================================================================
// Class Node
//======================================================================================

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node::Node() 
{
	fParent = kODNULL; 
}

//------------------------------------------------------------------------------
// Node::~Node
//
// Description
//------------------------------------------------------------------------------

Node::~Node() 
{
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

ODULong Node::Size() 
{
	ODULong size = 0;
	
	Node* node = this->FirstTopDown();
	while (node)
	{
		size++;
		node = node->NextTopDown(kODFrontToBack);
	}
	return size;
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::GetParent()
{
	return fParent;
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::GetFirstChild()
{
	return (Node*) this->First();
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::GetLastChild()
{
	return (Node*) this->Last();
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::GetNextSibling()
{
	Node* parent = this->GetParent();
	return parent ? parent->GetChildAfter(this) : kODNULL;
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::GetChildAfter(Node* node)
{
	return (Node*) this->After(*node);
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::GetChildBefore(Node* node)
{
	return (Node*) this->Before(*node);
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::GetPreviousSibling()
{
	Node* parent = this->GetParent();
	return parent ? parent->GetChildBefore(this) : kODNULL;
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

void Node::SetParent(Node* parent)
{
	fParent = parent;
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

void Node::AddChildFirst(Node* node)
{
	node->SetParent(this);
	this->AddFirst(node);
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

void Node::AddChildLast(Node* node)
{
	node->SetParent(this);
	this->AddLast(node);
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

void Node::AddChildBefore(Node& existing, Node* node)
{
	node->SetParent(this);
	this->LinkedList::AddBefore(existing, node);
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

void Node::AddChildAfter(Node& existing, Node* node)
{
	node->SetParent(this);
	this->LinkedList::AddAfter(existing, node);
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

void Node::RemoveChild(Node& child)
{
	this->LinkedList::Remove(child);
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::FirstTopDown()
{
	return this;
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::NextTopDown(ODSiblingOrder siblingOrder)
{
	Node* child;
	if (siblingOrder == kODFrontToBack)
		child = this->GetFirstChild();
	else
		child = this->GetLastChild();
	
	if (child)
		return child;
	else 
	{
		Node* sibling;
		if (siblingOrder == kODFrontToBack)
			sibling = this->GetNextSibling();
		else 
			sibling = this->GetPreviousSibling();

		if (sibling) 
			return  sibling;
		else 
			return this->GetNextAunt(siblingOrder);
	}
	return kODNULL;
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::GetNextAunt(ODSiblingOrder siblingOrder)
{
	Node* parent = this->GetParent();
	
	if (parent)
	{
		Node* sibling;
		if (siblingOrder == kODFrontToBack)
			sibling = parent->GetNextSibling();
		else
			sibling = parent->GetPreviousSibling();
		
		if (sibling) 
			return sibling;
		else 
			return parent->GetNextAunt(siblingOrder);
	}
	else
		return kODNULL;
}

#ifndef OD_BUG  // Method doesn't work, and no one in DR4 uses it.
//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::FirstBottomUp(ODSiblingOrder siblingOrder)
{
	Node* child;
	if (siblingOrder == kODFrontToBack)
		child = this->GetFirstChild();
	else
		child = this->GetLastChild();

	if (child)
		return child->FirstBottomUp(siblingOrder);
	else
		return this;
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* Node::NextBottomUp(ODSiblingOrder siblingOrder)
{
	Node* parent = this->GetParent();

	if (parent)
	{
		Node* sibling;
		if (siblingOrder == kODFrontToBack)
			sibling = parent->GetChildAfter(this);
		else
			sibling = parent->GetChildBefore(this);
		
		if (sibling)
			return sibling->FirstBottomUp(siblingOrder);
		else
			return parent;
	}
	else
		return kODNULL;	

	return kODNULL;
}
#endif

//======================================================================================
// NodeTraverser
//======================================================================================

//------------------------------------------------------------------------------
// NodeTraverser::NodeTraverser
//
// Description
//------------------------------------------------------------------------------

NodeTraverser::NodeTraverser(Node* root, 
					ODTraversalType traversalType, 
					ODSiblingOrder siblingOrder)
{
	fRoot = root;
	fCurrent = kODNULL;
	fTraversalType = traversalType;
	fSiblingOrder = siblingOrder;
}

//------------------------------------------------------------------------------
// NodeTraverser::NodeTraverser
//
// Description
//------------------------------------------------------------------------------

NodeTraverser::~NodeTraverser()
{
}

//------------------------------------------------------------------------------
// NodeTraverser::NodeTraverser
//
// Description
//------------------------------------------------------------------------------

Node*	NodeTraverser::First()
{
	if (fRoot)
	{
		if (fTraversalType == kODTopDown)
			fCurrent = this->FirstTopDown(fRoot);
		else if (fTraversalType == kODBottomUp)
			fCurrent = this->FirstBottomUp(fRoot,fSiblingOrder);
		else if (fTraversalType == kODChildrenOnly)
		{
			if (fSiblingOrder == kODFrontToBack)
				fCurrent = fRoot->GetFirstChild();
			else
				fCurrent = fRoot->GetLastChild();
		}
		return fCurrent;
	}
	else
		return kODNULL;
}

//------------------------------------------------------------------------------
// NodeTraverser::Next
//
// Description
//------------------------------------------------------------------------------

Node*	NodeTraverser::Next()
{
	if (fCurrent)
	{
		if (fTraversalType == kODTopDown)
			fCurrent = this->NextTopDown(fCurrent, fSiblingOrder);
		else if (fTraversalType == kODBottomUp)
			fCurrent = this->NextBottomUp(fCurrent, fSiblingOrder);
		else if (fTraversalType == kODChildrenOnly)
		{
			if (fSiblingOrder == kODFrontToBack)
				fCurrent = fCurrent->GetNextSibling();
			else
				fCurrent = fCurrent->GetPreviousSibling();
		}
		return fCurrent;
	}
	else
		return kODNULL;
}

//------------------------------------------------------------------------------
// NodeTraverser::SkipChildren
//
// Description
//------------------------------------------------------------------------------

void NodeTraverser::SkipChildren()
{
	Node* next = kODNULL;
	Node* skipTo = kODNULL;
	Node* test = kODNULL;

	if (fCurrent)
	{
		if (fTraversalType == kODTopDown)
		{
			if (fSiblingOrder == kODFrontToBack)
				next = fCurrent->GetNextSibling();
			else
				next = fCurrent->GetPreviousSibling();

			if (!next)
				next = fCurrent->GetNextAunt(fSiblingOrder);

			test = fCurrent;
			while ( test && (test != next) )
			{
				skipTo = test;
				test = this->NextTopDown(test, fSiblingOrder);
			}

			fCurrent = skipTo;
		}
	}
}

//------------------------------------------------------------------------------
// NodeTraverser::IsNotComplete
//
// Description
//------------------------------------------------------------------------------

ODBoolean	NodeTraverser::IsNotComplete()
{
	return (fCurrent != kODNULL);
}

//------------------------------------------------------------------------------
// NodeTraverser::FirstTopDown
//
// Description
//------------------------------------------------------------------------------

Node* NodeTraverser::FirstTopDown(Node* node)
{
	return node;
}

//------------------------------------------------------------------------------
// NodeTraverser::NextTopDown
//
// Description
//------------------------------------------------------------------------------

Node* NodeTraverser::NextTopDown(Node* node, ODSiblingOrder siblingOrder)
{
	Node* child;
	if (siblingOrder == kODFrontToBack)
		child = node->GetFirstChild();
	else
		child = node->GetLastChild();
	
	if (child)
		return child;
	else if (node != fRoot)
	{
		Node* sibling;
		if (siblingOrder == kODFrontToBack)
			sibling = node->GetNextSibling();
		else 
			sibling = node->GetPreviousSibling();

		if (sibling) 
			return  sibling;
		else 
			return this->GetNextAunt(node, siblingOrder);
	}
	return kODNULL;
}

//------------------------------------------------------------------------------
// NodeTraverser::GetNextAunt
//
// Description
//------------------------------------------------------------------------------

Node* NodeTraverser::GetNextAunt(Node* node, ODSiblingOrder siblingOrder)
{
	Node* parent = node->GetParent();
	
	if (parent && (parent != fRoot))
	{
		Node* sibling;
		if (siblingOrder == kODFrontToBack)
			sibling = parent->GetNextSibling();
		else
			sibling = parent->GetPreviousSibling();
		
		if (sibling) 
			return sibling;
		else 
			return this->GetNextAunt(parent, siblingOrder);
	}
	else
		return kODNULL;
}

//------------------------------------------------------------------------------
// NodeTraverser::FirstBottomUp
//
// Description
//------------------------------------------------------------------------------

Node* NodeTraverser::FirstBottomUp(Node* node, ODSiblingOrder siblingOrder)
{
	Node* child;
	if (siblingOrder == kODFrontToBack)
		child = node->GetFirstChild();
	else
		child = node->GetLastChild();

	if (child)
		return this->FirstBottomUp(child, siblingOrder);
	else
		return node;
}

//------------------------------------------------------------------------------
// Node::Node
//
// Description
//------------------------------------------------------------------------------

Node* NodeTraverser::NextBottomUp(Node* node, ODSiblingOrder siblingOrder)
{
	if (node == fRoot)
		return kODNULL;
		
	Node* parent = node->GetParent();

	if (parent)
	{
		Node* sibling;
		if (siblingOrder == kODFrontToBack)
			sibling = parent->GetChildAfter(node);
		else
			sibling = parent->GetChildBefore(node);
		
		if (sibling)
			return this->FirstBottomUp(sibling, siblingOrder);
		else
			return parent;
	}
	else
		return kODNULL;	

	return kODNULL;
}

