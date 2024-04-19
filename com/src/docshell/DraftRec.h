/* @(#)Z 1.4 com/src/docshell/DraftRec.h, odshell, od96os2, odos29646d 96/11/15 15:50:03 (96/10/29 09:23:53) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odshell
 *
 *   CLASSES:   DraftInfoRec
 *
 *   ORIGINS: 27
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

#ifndef _DRAFTREC_
#define _DRAFTREC_

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef SOM_ODDraft_xh
#include <Draft.xh>
#endif

//==============================================================================
// Constants
//==============================================================================

//==============================================================================
// Scalar Types
//==============================================================================

//==============================================================================
// Classes defined in this interface
//==============================================================================

class DraftInfoRec;

//==============================================================================
// Classes used by this interface
//==============================================================================

class DraftInfoRec 
{
public:
	DraftInfoRec();
	ODVMethod	~DraftInfoRec();
	ODNVMethod	void InitDraftInfoRec(Environment *ev, ODDraft *draft);


	ODMethod	DraftInfoRec*	GetPrev() 	{ return fPrev; }
	ODMethod	void		SetPrev(DraftInfoRec *prev)
							{fPrev = prev; }
	ODMethod	DraftInfoRec*	GetNext() 	{ return fNext; }
	ODMethod	void		SetNext(DraftInfoRec *next)
							{fNext = next; }
	ODMethod	ODDraft*	GetDraft()	{ return fDraft; }
	ODMethod	void		SetDraft(ODDraft *draft)
							{fDraft = draft; }
	ODMethod	ODDraftID	GetDraftID()	{ return fDraftID; }
	ODMethod	ODULong		GetDraftNum()	{ return fDraftNumber; }
	ODMethod	ODULong		GetSavedDate()	{ return fSavedDate; }
	ODMethod	ODIText*	GetModUser()	{ return fModUser; }
	ODMethod	void		SetModUser(ODIText *modUser)
							{ fModUser = modUser; }
	ODMethod	ODIText*	GetComment()	{ return fComment; }
	ODMethod	void		SetComment(ODIText *comment)	
							{ fComment = comment; }

protected:

private:
	DraftInfoRec*		fPrev;
	DraftInfoRec*		fNext;
	ODDraft*		fDraft;
	ODDraftID		fDraftID;
	ODULong			fDraftNumber;
	ODULong			fSavedDate;
	ODIText*		fModUser;
	ODIText*		fComment;
};

#endif	// _DRAFTREC_
