/* @(#)Z 1.3 com/src/ui/DispTabl.cpp, odui, od96os2, odos29646d 96/11/15 15:28:01 (96/07/15 18:26:06) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odui
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
/*
	File:		DispTabl.cpp

	Contains:	Implementation of class DispatchTable, which is private
				to the implementation of ODDispatcher

	Owned by:	Richard Rodseth

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <4>	 5/26/95	RR		#1251403: Multithreading naming support
		 <3>	 5/10/95	RR		# 1245459 Don't delete fDispatchModule in
																		~DispatchInfo
		 <2>	  5/2/95	RR		# 1245459 Delete fDispatchModule in
									~DispatchInfo
		 <1>	 5/13/94	RR		first checked in
		 <1>	 5/13/94	RR		first checked in
		<13>	  2/7/94	NP		Tiger Team doings.
		<12>	 12/3/93	T‚		Stop including ODError.h, it is included
									as ErrorDef.h inside Except.h
		<11>	 12/2/93	RR		Use new eventType definition
		<10>	 9/24/93	RR		#pragma segment
		 <9>	 8/20/93	RR		Fixed array indexing bug
		 <8>	 8/19/93	RR		Remove redundant workaround in destructor
		 <7>	 8/19/93	RR		Work around apparent CFront problems with
									delete[] in ~DispatchTable
		 <6>	 8/19/93	NP		Work around CFront's apparent problem with
									delete 0
		 <5>	 8/18/93	RR		Added GetDispatchInfo/UpdateDispatchInfo
		 <4>	 8/18/93	RR		Use DictionaryList class
		 <2>	 8/10/93	RCR		Use OrderedCollection instead of linked list, for monitors
		 <1>	 8/10/93	RCR		first checked in

	To Do:
*/

#ifndef _DISPTABL_
#include "DispTabl.h"
#endif

#ifndef _DISPMOD_
#include "DispMod.xh"
#endif

#ifndef _ORDCOLL_
#include "OrdColl.h"
#endif

#ifndef _DICTLIST_
#include "DictList.h"
#endif

#ifndef _EXCEPT_
#include "Except.h"
#endif

#pragma segment ODDispatcher

//=====================================================================================
// Private Class: DispatchInfo
//=====================================================================================

class DispatchInfo 
{
public:
	
	DispatchInfo();
	
		// Constructor
		
	~DispatchInfo();
	
		// Destructor
		
	void SetModule(ODDispatchModule* module);
	ODDispatchModule* GetModule();
	void AddMonitor(ODDispatchModule* monitor);
	void RemoveMonitor(ODDispatchModule* monitor);
	OrderedCollection* GetMonitors();

protected:
	ODDispatchModule* fDispatchModule;
	OrderedCollection* fMonitors;
};

//-------------------------------------------------------------------------------------
// DispatchInfo::DispatchInfo
//
// Description
//-------------------------------------------------------------------------------------

DispatchInfo::DispatchInfo()
{
	fDispatchModule = kODNULL;
	fMonitors = kODNULL;
}

//-------------------------------------------------------------------------------------
// DispatchInfo::~DispatchInfo
//
// Description
//-------------------------------------------------------------------------------------

DispatchInfo::~DispatchInfo()
{
	delete fMonitors;
	// Don't delete fDispatchModule, because many entries in the table
	// point to the same module
}

//-------------------------------------------------------------------------------------
// DispatchInfo::SetModule
//
// Description
//-------------------------------------------------------------------------------------

void DispatchInfo::SetModule(ODDispatchModule* module)
{
	fDispatchModule = module;
}

//-------------------------------------------------------------------------------------
// DispatchInfo::GetModule
//
// Description
//-------------------------------------------------------------------------------------

ODDispatchModule* DispatchInfo::GetModule()
{
	return fDispatchModule;
}

//-------------------------------------------------------------------------------------
// DispatchInfo::AddMonitor
//
// Exceptions thrown:
// 		kODErrOutOfMemory
//-------------------------------------------------------------------------------------

void DispatchInfo::AddMonitor(ODDispatchModule* monitor)
{
	if (monitor)
	{
		if (!fMonitors)
			fMonitors = new OrderedCollection;
		THROW_IF_NULL(fMonitors);
		
		if (!(fMonitors->Contains((ElementType) monitor)))
			fMonitors->AddLast((ElementType) monitor);
	}
}
	
//-------------------------------------------------------------------------------------
// DispatchInfo::RemoveMonitor
//
// Description
//-------------------------------------------------------------------------------------

void DispatchInfo::RemoveMonitor(ODDispatchModule* monitor)
{
	if (fMonitors)
		fMonitors->Remove((ElementType) monitor);
}

//-------------------------------------------------------------------------------------
// DispatchInfo::GetMonitors
//
// Description
//-------------------------------------------------------------------------------------

OrderedCollection* DispatchInfo::GetMonitors()
{
	return fMonitors;
}

//=====================================================================================
// Class DispatchTable
//=====================================================================================

//-------------------------------------------------------------------------------------
// DispatchTable::DispatchTable
//
// Constructor. Initializes the array entries to NULL.
//-------------------------------------------------------------------------------------

DispatchTable::DispatchTable()
{
	for (short i = 0; i <= kODLastEvent; i++)
		fDispatchInfo[i] = kODNULL;
	fOverflowDispatchInfo = kODNULL;
}

//-------------------------------------------------------------------------------------
// DispatchTable::~DispatchTable
//
// Description
//-------------------------------------------------------------------------------------

DispatchTable::~DispatchTable()
{
	for (short i = 0; i <= kODLastEvent; i++)	
		delete fDispatchInfo[i];
	//delete[] fDispatchInfo;	// CFront has a problem with this?
	if (fOverflowDispatchInfo)
		fOverflowDispatchInfo->DeleteValues();
	delete fOverflowDispatchInfo;
}

//-------------------------------------------------------------------------------------
// DispatchTable::AddMonitor
//
// Description
//-------------------------------------------------------------------------------------

void DispatchTable::AddMonitor(ODEventType eventType, 
							   ODDispatchModule* dispatchModule)
{
	// Should check if dispatchModule is already installed as a regular module?
	
	DispatchInfo* info = this->GetDispatchInfo(eventType);
	if (info == kODNULL)
	{
		info = new DispatchInfo;
		THROW_IF_NULL(info);
		this->UpdateDispatchInfo(eventType,info);
	}
	info->AddMonitor(dispatchModule);
}

//-------------------------------------------------------------------------------------
// DispatchTable::RemoveMonitor
//
// Description
//-------------------------------------------------------------------------------------

void DispatchTable::RemoveMonitor(ODEventType eventType, ODDispatchModule* dispatchModule)
{
	DispatchInfo* info = this->GetDispatchInfo(eventType);
	if (info)
		info->RemoveMonitor(dispatchModule);
}

//-------------------------------------------------------------------------------------
// DispatchTable::AddDispatchModule
//
// Description
//-------------------------------------------------------------------------------------

void DispatchTable::AddDispatchModule(ODEventType eventType, 
							   ODDispatchModule* dispatchModule)
{
	// To Do: THROW exception if already installed

	DispatchInfo* info = this->GetDispatchInfo(eventType);
	
	if (info == kODNULL)
	{
		info = new DispatchInfo;
		THROW_IF_NULL(info);
		this->UpdateDispatchInfo(eventType,info);			
	}
	info->SetModule(dispatchModule);
}

//-------------------------------------------------------------------------------------
// DispatchTable::RemoveDispatchModule
//
// Description
//-------------------------------------------------------------------------------------

void DispatchTable::RemoveDispatchModule(ODEventType eventType)
{
	DispatchInfo* info = this->GetDispatchInfo(eventType);
	if (info)
		info->SetModule(kODNULL);
}

//-------------------------------------------------------------------------------------
// DispatchTable::GetDispatchInfo
//
// Description
//-------------------------------------------------------------------------------------

DispatchInfo* DispatchTable::GetDispatchInfo(ODEventType eventType)
{
	DispatchInfo* info = kODNULL;

	if (eventType <= kODLastEvent)
		info = fDispatchInfo[eventType];
	else if (fOverflowDispatchInfo)
		info = (DispatchInfo*) fOverflowDispatchInfo->ValueAtKey((KeyType) eventType);
	return info;
}

//-------------------------------------------------------------------------------------
// DispatchTable::UpdateDispatchInfo
//
// Description
//-------------------------------------------------------------------------------------
	
void DispatchTable::UpdateDispatchInfo(ODEventType eventType, DispatchInfo* info)
{
	if (eventType <= kODLastEvent)
		fDispatchInfo[eventType] = info;
	else
	{
		if (!fOverflowDispatchInfo)
			fOverflowDispatchInfo = new DictionaryList;
		fOverflowDispatchInfo->AddPair((KeyType) eventType, (ValueType) info);
	}
}

//-------------------------------------------------------------------------------------
// DispatchTable::GetDispatchModule
//
// Description
//-------------------------------------------------------------------------------------

ODDispatchModule* DispatchTable::GetDispatchModule(ODEventType eventType)
{
	ODDispatchModule* module = kODNULL;
	DispatchInfo* info = kODNULL;
	
	if (eventType <= kODLastEvent)
		info = fDispatchInfo[eventType];
	else if (fOverflowDispatchInfo)
		info = (DispatchInfo*) fOverflowDispatchInfo->ValueAtKey((KeyType) eventType);
	if (info)
		module = info->GetModule();
	return module;
}

//-------------------------------------------------------------------------------------
// DispatchTable::GetMonitors
//
// Description
//-------------------------------------------------------------------------------------

OrderedCollection* DispatchTable::GetMonitors(ODEventType eventType)
{
	OrderedCollection* monitors = kODNULL;
	DispatchInfo* info = kODNULL;
	
	if (eventType <= kODLastEvent)
		info = fDispatchInfo[eventType];
	else if (fOverflowDispatchInfo)
		info = (DispatchInfo*) fOverflowDispatchInfo->ValueAtKey((KeyType) eventType);
	if (info)
		monitors = info->GetMonitors();
	return monitors;
}


