/* @(#)Z 1.2 com/src/ui/IdleList.h, odui, od96os2, odos29646d 96/11/15 15:28:05 (96/07/15 18:26:50) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
 *
 *   CLASSES:   IdleInfo
 *		IdleList
 *		IdleListIterator
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
	File:		IdleList.h

	Contains:	Interface to IdleInfo, IdleList and IdleListIterator classes.

	Owned by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <6>	 5/25/95	jpa		List.h --> LinkList.h [1253324]
		 <5>	  4/4/95	RR		1228161 Pass ev to SetIdleFreq
		 <4>	 1/31/95	RR		# 1206909 Pass ev to
									RemoveUnregisteredIdlers
		 <3>	 9/30/94	RR		#1167950 Allow unregistering while idling
		 <2>	 9/20/94	RR		Added ev parameter to AddIdle and
									RemoveIdle. #1154046 Moved ref counting
									into IdleList methods so that an
									unregistered part is not released.
		 <1>	 5/13/94	RR		first checked in
		 <5>	 3/15/94	MB		Changes to support SCpp/ASLM builds,
									#1150864.
		 <4>	  2/9/94	NP		Tiger Team cleanup.
		 <3>	 1/26/94	RR		Include ODTypes
		 <2>	12/20/93	RR		interface takes part/frame pairs rather
									than frames
		 <1>	11/16/93	RR		first checked in

	To Do:
	In Progress:
*/

#ifndef _IDLELIST_
#define _IDLELIST_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _LINKLIST_
#include <LinkList.h>
#endif

//==============================================================================
// Theory of Operation
//==============================================================================

//==============================================================================
// Scalar Types
//==============================================================================

typedef ODULong ODTicks; 

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class IdleInfo; 
class IdleList;				
class IdleListIterator;				

//=====================================================================================
// Classes used by this interface
//=====================================================================================

class ODFrame;
class ODPart;

//=====================================================================================
// Global Variables
//=====================================================================================

//======================================================================================
// Class IdleInfo
//======================================================================================

class IdleInfo : public Link
{
public:
	IdleInfo(ODPart* part, ODFrame* frame, ODIdleFrequency frequency);
	virtual ~IdleInfo();

	ODBoolean NeedsIdle(ODTicks ticks);
	ODTicks NextIdle(ODTicks ticks);
	
	ODPart* GetPart() { return fPart; }
	ODFrame* GetFrame() { return fFrame; }
	ODIdleFrequency GetIdleFrequency() { return fIdleFrequency; }
	void SetIdleFrequency(ODIdleFrequency frequency) { fIdleFrequency = frequency; }
	ODTicks GetLastIdle() { return fLastIdle; }
	void SetLastIdle(ODTicks lastIdle) { fLastIdle = lastIdle; }
	ODBoolean ShouldRemove() { return fRemove; }
	void SetShouldRemove(ODBoolean shouldRemove) { fRemove = shouldRemove; }

private:	
	ODPart*			fPart;
	ODFrame* 			fFrame;
	ODIdleFrequency 	fIdleFrequency;
	ODSLong 			fLastIdle;
	ODBoolean		fRemove;
};

//=====================================================================================
// IdleList
//=====================================================================================

class IdleList
{
	friend class IdleListIterator;
	
public:
	IdleList();
	virtual ~IdleList();
	
	void AddIdle(Environment* ev, ODPart* part, ODFrame* frame, ODIdleFrequency frequency); 
	
	void RemoveIdle(Environment* ev, ODPart* part, ODFrame* frame); 
	
	void SetIdleFrequency(Environment* ev, ODPart* part, ODFrame* frame, ODIdleFrequency frequency); 

	void RemoveUnregisteredIdlers(Environment* ev);
	
private:
	LinkedList fImplementation;
};

//=====================================================================================
// IdleListIterator
//=====================================================================================

class IdleListIterator
{
public:

	IdleListIterator(IdleList* idleList);
	
		// Constructor
		
	ODVMethod ~IdleListIterator();
	
		// Destructor
	
	ODMethod IdleInfo* First();
	
		// Returns the first element of the set
		
	ODMethod IdleInfo* Next();
	
		// Returns the next element in the set
		
	ODMethod ODBoolean IsNotComplete();
	
		// Returns TRUE as long as there are more elements in the set
	
private:
	
	LinkedListIterator fIterator;
	
};

#endif // _IDLELIST_
