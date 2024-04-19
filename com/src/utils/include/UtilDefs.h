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
// @(#) 1.3 com/src/utils/include/UtilDefs.h, odutils, od96os2, odos29646d 7/15/96 18:02:31 [ 11/15/96 15:29:04 ]
/*
	File:		UtilDefs.h

	Contains:	constants used in both the code & resource of Info Utilities

	Owned by:	Tantek ‚elik

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	
	In Progress:
		
*/

#ifndef _UTILDEFS_
#define _UTILDEFS_

/* If you update kODUtilitiesRsrcBase do it in RsrcBase.h also. */
#define kODUtilitiesRsrcBase	600

// String used by ODGetCategory
#define kODPartInfoStrUnknownID			(kODUtilitiesRsrcBase+0)

// Other Strings used by InfoUtil
#define	kODInfoUtilStrsID				(kODUtilitiesRsrcBase+1)
#define kODStrIndNoKinds					1
#define kODStrIndNoEditors					2

// Strings used by shell and platformfile
#define kCopyDefaultNameNoNumStrID		(kODUtilitiesRsrcBase + 2)
#define kCopyDefaultNameWithNumStrID	(kODUtilitiesRsrcBase + 3)
#define kConcatDefaultNameNoNumStrID	(kODUtilitiesRsrcBase + 4)


// For Part Name renaming (when it is the root part of the document)
// Used by ODSetPartName
/* Name Conflict dialog defines */
#define kNameConflictDlgID        		(kODUtilitiesRsrcBase+0)
#define kNameConflictCancelBtn				1
#define kNameConflictReplaceBtn		   		2
#define kNameConflictTextItem				3
#define kNameConflictDefaultButtonItem		4

/* File Replace error alert defines */
#define kFilelockedAlrtID        		(kODUtilitiesRsrcBase+1)
#define kOkBtn		   						1

/* SOM error alert/dialog from ODDebug.cpp: */
#define kSOMErrorAlertID				(kODUtilitiesRsrcBase+2)
#define kSOMErrorDlogID					(kODUtilitiesRsrcBase+3)
#define kSOMErrorDlogTextItem			   3
#define kSOMErrorDlogDebugButton		   2
#define kSOMErrorDlogFontInfo			(kODUtilitiesRsrcBase+4)


#endif // _UTILDEFS_
