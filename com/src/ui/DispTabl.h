/* @(#)Z 1.3 com/src/ui/DispTabl.h, odui, od96os2, odos29646d 96/11/15 15:29:30 (96/07/15 18:26:10) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
 *
 *   CLASSES:   DispatchTable
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
	File:		DispTabl.h

	Contains:	Definition of class DispatchTable, which is private to the implementation of
				ODDispatcher.

	Owned by:	Richard Rodseth

	Copyright:	© 1992 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <2>	  9/8/95	jpa		Use official error number range for private
									error [1281354]
		 <1>	 5/13/94	RR		first checked in
		 <9>	  2/9/94	NP		Tiger Team cleanup.
		 <8>	 1/15/94	RR		WinState.h->WinStat.h,
									Dispatch.h->Disptch.h
		 <7>	 12/2/93	RR		Use new eventType definition
		 <6>	 8/20/93	RR		Fixed array indexing bug
		 <5>	 8/18/93	RR		Added GetDispatchInfo/UpdateDispatchInfo
		 <4>	 8/18/93	RR		Use DictionaryList class
		 <2>	8/17/93	RCR		Use OrderedCollection instead of linked list, for monitors
		 <1>	8/10/93	RCR		first checked in

	To Do:
	In Progress:
		
*/

#ifndef _DISPTABL_
#define _DISPTABL_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

//=====================================================================================
// Theory of Operation
//=====================================================================================

/*
	See Disptch.h

	Contains an array of dispatch modules for the first 256 event codes. If there are 
	any more, they go in an overflow linked list. A Windows implementation might  use 
	a hash table instead, since there are so many more messages.

*/

//=====================================================================================
// Constants
//=====================================================================================

const ODEventType kODLastEvent = 255; //	Index of last entry in the dispatch table

//=====================================================================================
// Scalar Types
//=====================================================================================

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class DispatchTable;

//=====================================================================================
// Classes used by this interface
//=====================================================================================

class DispatchInfo;
class OrderedCollection;
class DictionaryList;
class ODDispatchModule;

//=====================================================================================
// Global Variables
//=====================================================================================

//=====================================================================================
// Class DispatchTable
//=====================================================================================

class DispatchTable
{
public:

	DispatchTable();
	
		// Constructor
		
	~DispatchTable();
	
		// Destructor

	void AddMonitor(ODEventType eventType, 
					ODDispatchModule* dispatchModule);
					
	void RemoveMonitor(ODEventType eventType,ODDispatchModule* dispatchModule);
	
	void AddDispatchModule(ODEventType eventType, 
						   ODDispatchModule* dispatchModule);
						   
	void RemoveDispatchModule(ODEventType eventType);
	
	ODDispatchModule* GetDispatchModule(ODEventType eventType);

	OrderedCollection* GetMonitors(ODEventType eventType);
	
protected:

	DispatchInfo* GetDispatchInfo(ODEventType eventType);
	void UpdateDispatchInfo(ODEventType eventType, DispatchInfo* info);
	
private:

	DispatchInfo* fDispatchInfo[kODLastEvent + 1]; // An array for the first 256 event types
	DictionaryList* fOverflowDispatchInfo; 	   	  // A dictionary for those that don't 
												  // fit in the array
										
};

#endif // _DISPTABL_
