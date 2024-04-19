/* @(#)Z 1.14 com/src/docshell/DraftRec.cpp, odshell, od96os2, odos29646d 96/11/15 15:50:03 (96/10/29 09:23:48) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odshell
//
//   CLASSES: none
//
//   ORIGINS: 27
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

//------------------------------------------------------------------------------
// DraftRec.cpp
//
//	This file provides the implementation of the DraftInfoRec class.
//	An instance of this class is created for each draft listed in
//	the draft history window.
//------------------------------------------------------------------------------
#include "DraftRec.h"
#include "DocShell.h"
#include <ODUtils.h>
#include <IText.h>
#include <StorUtil.h>
#include <StdTypIO.h>
#include <TempObj.h>
#include <StdProps.xh>
#include <StdTypes.xh>
#include <StorageU.xh>
#include <Document.xh>


//------------------------------------------------------------------------------
// DraftInfoRec
//------------------------------------------------------------------------------
DraftInfoRec::DraftInfoRec()
{
	fPrev			= kODNULL;
	fNext			= kODNULL;
	fDraft			= kODNULL;
	fDraftID		= kODNULL;
	fDraftNumber		= 0;
	fSavedDate		= kODNULL;
	fComment		= kODNULL;
	fModUser		= kODNULL;
}


//------------------------------------------------------------------------------
// ~DraftInfoRec
//
//	Delete DraftInfoRec which includes deleting memory allocated for
//      user and comment information and releasing the draft acquired.
//------------------------------------------------------------------------------
DraftInfoRec::~DraftInfoRec()
{
	Environment* ev = somGetGlobalEnvironment();

	DisposeIText(fModUser);
	DisposeIText(fComment);

        try
        {
	   // Release draft
	   // Note: it is acquired in InitDraftInfoRec
	   ODReleaseObject(ev, fDraft);
        }
        catch (ODException _exception)
        {
           LOG("Exception occured in DraftInfoRec::~DraftInfoRec");
           throw;
        }

}


//------------------------------------------------------------------------------
// InitDraftInfoRec
//
//	Retrieve information belonging to this draft from its draft
//	properties
//------------------------------------------------------------------------------
void DraftInfoRec::InitDraftInfoRec(Environment *ev, ODDraft *draft)
{

    try
    {
	TempODStorageUnit su = draft->AcquireDraftProperties(ev);

	// Acquire reference to draft.
	// Note: reference to be released by the destructor
        fDraft = draft->GetDocument(ev)->AcquireDraft(ev, kODDPReadOnly,
						kODNULL, draft, kODPosSame,
						kODFalse);

	// Retrieve draft ID
	fDraftID = draft->GetID(ev);

	// Retrieve user
        fModUser = ODGetITextProp(ev, su, kODPropModUser, 
				kODPlatformIText , kODNULL);

	// Retrieve comment
        fComment = ODGetITextProp(ev, su, kODPropDraftComment,
				kODPlatformIText, kODNULL);

	// Retrieve creation date
	fSavedDate = ODGetTime_TProp(ev, su, kODPropDraftSavedDate, kODTime_T);

	// Retrieve draft number
        fDraftNumber = ODGetULongProp(ev, su, kODPropDraftNumber, kODULong);

    }
    catch (ODException _exception)
    {
        LOG("Exception occured in DraftInfoRec::InitDraftInfoRec");
	if (fModUser)
		DisposeIText(fModUser);
	if (fComment)
		DisposeIText(fComment);
	if (fDraft)
           SaveAndRestoreEv1(fDraft);
        throw;
    }

}
