/* @(#)Z 1.5 com/src/storage/LinkDefs.h, oddataxfer, od96os2, odos29646d 96/11/15 15:25:51 (96/10/29 09:31:15) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: oddataxfer
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
	File:		LinkDefs.h

	Contains:	#defines for data interchange dialogs

	Owned by:	Craig Carper

	Copyright:	© 1994 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<13>	 8/23/95	CC		1278840: Added kPasteAsEmbedEditorText
									item.
		<12>	 8/11/95	jpa		Number rsrcs from subsystem base [1276412]
		<11>	 7/28/95	CC		1242576: Removed #defines for Others
									dialog.
		<10>	 4/27/95	CC		1243348: Removed BreakLinkAlert resources.
		 <9>	 3/27/95	CC		1229640: Added items for editor popup and
									static text.
		 <8>	  3/3/95	CC		1223861 Added kLinkStatusAlertID and
									kLinkEditionAlertID.
		 <7>	 2/24/95	CC		1153783: Changed kODLinkMgrEditionSuffixID
									to kODLinkMgrEditionTemplateID.
		 <6>	 2/15/95	CC		1196831 Added kODLinkInfoSizeInBytes and
									kODLinkInfoSizeInK.
									1220324 Adjusted constants for changes to
									LinkDlgs.r
		 <5>	  2/7/95	CC		1211295: Added kPasteAsKindLabel
									kPasteAsKindText.
		 <4>	  2/1/95	CC		1153347: Added symbolic constant for 'STR '
									resource.
		 <3>	 1/10/95	CC		1153930: Added constants for "Others"
									dialog (translation choices); new constants
									for Paste As dialog.
		 <2>	 9/20/94	CC		1160232 - Added constants for link cycle
									Alert dialog.
		 <1>	 6/24/94	CC		first checked in

		------------------- CC ---- Converted to SOM

		 <2>	 3/17/94	CC		Added defs for link info dialogs. (1151329)
		 <1>	 3/14/94	CC		first checked in
		 <0>	 3/14/94	CC		Defines for Paste As dialog (1144253).

	To Do:
*/

#ifndef _LINKDEFS_
#define _LINKDEFS_

#ifdef _PLATFORM_MACINTOSH_
#ifndef _RSRCBASE_
#include "RsrcBase.h"
#endif
#endif // _PLATFORM_MACINTOSH_

#define kPasteAsDlgID					(kODStorageRsrcBase + 0)
#define kLinkSrcInfoDlgID				(kODStorageRsrcBase + 1)
#define kLinkDstInfoDlgID				(kODStorageRsrcBase + 2)
#define kLinkUpdateAlertID				(kODStorageRsrcBase + 3)
#define kLinkStatusAlertID				(kODStorageRsrcBase + 4)
#define kLinkEditionAlertID				(kODStorageRsrcBase + 5)
#define kLinkDlgsppatID					(kODStorageRsrcBase + 6)

// Resources used by Paste As dialog items
#define kPasteAsMergePictID				(kODStorageRsrcBase + 7)
#define kPasteAsEmbedPictID				(kODStorageRsrcBase + 8)
#define kPasteAsEmbedPopupCntl			(kODStorageRsrcBase + 9)
#define kPasteAsEmbedPopupMenu			(kODStorageRsrcBase + 10)
#define kPasteAsKindPopupCntl			(kODStorageRsrcBase + 11)
#define kPasteAsKindPopupMenu			(kODStorageRsrcBase + 12)
#define kPasteAsCheckboxCtrlID			(kODStorageRsrcBase + 13)
#define kPasteAsAutoRadioBtnCtrlID		(kODStorageRsrcBase + 14)
#define kPasteAsManualRadioBtnCtrlID	(kODStorageRsrcBase + 15)
#define kPasteAsMergeRadioBtnCtrlID		(kODStorageRsrcBase + 16)
#define kPasteAsEmbedRadioBtnCtrlID		(kODStorageRsrcBase + 17)
#define kPasteAsEditorPopupCntl			(kODStorageRsrcBase + 18)
#define kPasteAsEditorPopupMenu			(kODStorageRsrcBase + 19)

// Resources used by Link Info dialog items
#define kManualRadioBtnCtrlID			(kODStorageRsrcBase + 20)
#define kAutomaticRadioBtnCtrlID		(kODStorageRsrcBase + 21)
#define kOnSaveRadioBtnCtrlID			(kODStorageRsrcBase + 22)

// Strings used by Paste As dialog
#define kODPasteAsNoUserKindID			(kODStorageRsrcBase + 23)

// Strings used by Link Info dialogs
#define kODLinkInfoStrUnknownID			(kODStorageRsrcBase + 24)

// String resources used by the Link Manager
#define kODLinkMgrEditionTemplateID		(kODStorageRsrcBase + 25)
#define kODLinkMgrCannotShowSourceID	(kODStorageRsrcBase + 26)

// Items in Paste As dialog
#define kPasteAsOKBtn				 1
#define kPasteAsCancelBtn			 2
#define kPasteAsLinkCheckbox		 3
#define kPasteAsUpdateText			 4
#define kPasteAsUpdateDisabledText	 5
#define kPasteAsAutomaticRadioBtn	 6
#define kPasteAsManualRadioBtn		 7
#define kPasteAsHorizSep			 8
#define kPasteAsDestinationText		 9
#define kPasteAsMergePict			10
// User item to disable previous item
#define kPasteAsMergeRadioBtn		12
#define kPasteAsEmbedPict			13
// User item to disable previous item
#define kPasteAsEmbedRadioBtn		15
#define kPasteAsEmbedPopup			16
#define kPasteAsKindPopup			17
#define kPasteAsKindLabel			18
#define kPasteAsKindText			19
#define kPasteAsEditorPopup			20
#define kPasteAsEditorLabel			21
#define kPasteAsEditorText			22
#define kPasteAsEmbedEditorText		23
#define kPasteAsDefaultButtonItem	24

// Items in the Paste As popup menu
#define kODPasteAsFrameMenuItem			1
#define kODPasteAsLargeIconMenuItem		2
#define kODPasteAsSmallIconMenuItem		3	
#define kODPasteAsThumbnailMenuItem		4

// Items in Link Source and Link Destination Info dialogs
#define kLinkInfoOKBtn					 1
#define kLinkInfoCancelBtn				 2
#define kLinkInfoDefaultButtonItem	 	 3
#define kLinkInfoSeparator               4
#define kLinkInfoFindSrcBtn				 5
#define kLinkInfoUpdateBtn    			 6
#define kLinkInfoBreakLinkBtn			 7
#define kLinkInfoManualRadioBtn			 8
#define kLinkInfoOnSaveRadioBtn			 9
#define kLinkInfoAutoRadioBtn			10
#define kLinkInfoModificationDate		11
#define kLinkInfoCreationDate			13
#define kLinkInfoKind					15
#define kLinkInfoStayInDialog			17

// Items in Link Update alert
#define kLinkUpdateContinueBtn			1
#define kLinkUpdateStopBtn				2

#endif // _LINKDEFS_
