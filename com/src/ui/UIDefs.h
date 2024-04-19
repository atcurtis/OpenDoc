/* @(#)Z 1.2 com/src/ui/UIDefs.h, odui, od96os2, odos29646d 96/11/15 15:28:19 (96/07/15 18:27:30) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odui
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
	File:		UIDefs.h

	Contains:	Constants which are used by code & resources for the UI Subsystem

	Owned by:	Richard Rodseth

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <3>	 8/11/95	jpa		Number rsrcs from subsystem base [1276412]
		 <2>	 6/29/95	jpa		Added kODDesktopTitleStrID [1262681]
		 <1>	 5/4/95	eeh		1242889: First Checked In.

	In Progress:
		
*/

#ifndef _UIDEFS_
#define _UIDEFS_

#ifndef _RSRCBASE_
#include "RsrcBase.h"
#endif

/* the " Draft " string that gets used for drafts opened into windows */

#define kODDraftTextResID		(kODUIRsrcBase + 0)

#define kODDesktopTitleStrID	(kODUIRsrcBase + 1)

#endif	/* _UIDEFS_ */
