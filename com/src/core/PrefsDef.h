/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odcore
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
/* @(#)Z 1.3 com/src/core/PrefsDef.h, odcore, od96os2, odos29646d 96/11/15 15:24:55 (96/10/30 20:21:55) */
/*
	File:		PrefsDef.h

	Contains:	Constants for prefs

	Owned by:	Caia Grisar

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <9>	 8/30/95	CG		#1257569 FB3:Added kODPrefsFolderNameStrID.
		 <8>	 8/11/95	jpa		Number rsrcs from subsystem base [1276412]
		 <7>	 6/22/95	jpa		Added kODBadAliasInEditorsAlertID [1257556]
		 <6>	  6/9/95	CG		#1203571 implemented nmap caching
									mechanism.
		 <5>	 5/16/95	jpa		Added kODScanEditorsDialogID [1240150]
		 <3>	 2/14/95	TJ		Fixed copyright date.
		 <2>	 2/14/95	jpa		Added kODEditorHintFileName [1205141]
		 <1>	  2/2/95	CG		first checked in
	To Do:
*/

#ifndef _PREFSDEF_
#define _PREFSDEF_

#ifndef _RSRCBASE_
#include "RsrcBase.h"
#endif

#define kODPrefsFileNameStrID 		(kODCoreRsrcBase + 0)
#define kODEditorsFldrNameStrID 	(kODCoreRsrcBase + 1)
#define kODLibsFldrNameStrID		(kODCoreRsrcBase + 2)
#define kODPrefsFolderNameStrID 	(kODCoreRsrcBase + 3)

#define kODEditorHintFileName "\pOpenDocInfo\r"
// This is the invisible file stored on the root of a volume whose data fork
// contains the name of the "Editors" folder. The name of this file is purposely
// NOT localized so that it can be found on all hard disks, regardless of their
// foreign tongues.

#define kODCacheFileName "\pODCacheFile\r"
// This is the invisible cache file stored at the top of each folder structure
// whose files contain nmap resources. The name of this file is purposely
// NOT localized so that it can be found on all hard disks, regardless of their
// foreign tongues.


#define kODScanEditorsDialogID			(kODCoreRsrcBase + 3)
#define kODBadAliasInEditorsAlertID		(kODCoreRsrcBase + 4)

#endif
