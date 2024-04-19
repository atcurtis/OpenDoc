/* @(#)Z 1.4 com/src/bento/PrmRslvr.h, odstorage, od96os2, odos29646d 96/11/15 15:26:35 (96/10/29 09:13:20) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
 *
 *   CLASSES:   PromiseResolver
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
	File:		PrmRslvr.h

	Contains:	PromiseResolver header

	Written by:	Vincent Lo

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 4/10/95	VL		1236490: Made GetPromiseValue work during
									FulfillPromise.
		 <2>	 3/14/95	VL		1225208,1225201: Added IsSettingPromise to
									ensure that we don't call ResolvePromise
									while setting a promise.
		 <1>	 7/26/94	VL		first checked in
	To Do:
*/

#ifndef _PRMRSLVR_
#define _PRMRSLVR_

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef __CM_API__
#include <CMAPI.h>
#endif

typedef struct {
	ODPart*				sourcePart;
	ProcessSerialNumber sourcePSN;
} ODValueRefCon;

class CMStorageUnit;
class ODPart;

class PromiseResolver 
{
public:

	PromiseResolver();
	
	~PromiseResolver();
	
	void InitPromiseResolver(CMStorageUnit* su);

	ODBoolean IsPromiseValue(Environment* ev);

	void SetSourcePart(Environment* ev,
						ODPart *sourcePart);

	ODPart* GetSourcePart(Environment* ev);

	void ClearPromise(Environment* ev);

	void ResolvePromise(Environment* ev);

	ODULong GetPromiseCount();
	
	void SettingPromise();
	
	void DoneSettingPromise();
	
	ODBoolean IsSettingPromise();
	
protected:

	ODPtr GetPromiseInfo(CMValue curValue);

	void SetPromiseInfo(CMValue curValue, ODPtr theInfo);
	
	void IncrementPromiseCount();
	
	void DecrementPromiseCount();
	
	ODULong		fPromiseCount;
	CMStorageUnit*	fSU;
	ODBoolean	fSettingPromise;
	ODValueRefCon*	fResolvingPromiseInfo;
};


#endif	// _PRMRSLVR_
