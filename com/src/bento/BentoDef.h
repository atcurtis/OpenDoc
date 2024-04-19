/* @(#)Z 1.4 com/src/bento/BentoDef.h, odstorage, od96os2, odos29646d 96/11/15 15:26:07 (96/10/29 09:10:30) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odstorage
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
	File:		BentoDef.h

	Contains:	defines for Bento code and resources

	Owned by:	Vincent Lo

	Copyright:	© 1993 - 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<13>	10/20/95	VL		1293256: Removed
									kODErrBentoInvalidVersionList.
		<12>	10/13/95	EL		1287340: Use standard ISO prefix
		<11>	  9/8/95	jpa		Use official error number range. [1281354]
		<10>	 8/24/95	EL		1276818: move Bento container suite
									property to BentoDef.h. 1278212: move
									private error code to BentoDef.h.
		 <9>	 8/21/95	VL		1278330, 1278315: Error code cleanup. Added
									kODErrCannotGetDraftVersion.
		 <8>	 5/10/95	CG		#1243599	RemovekNMAPid from StdDefs.idl.
		 <7>	 2/10/95	VL		1205627: Added ODBentoUpdateContainer.
		 <6>	11/17/94	CG		#1181489: Fixed kODMemoryContainerID to
									ODMemContainer.
		 <5>	11/14/94	VL		1188257: Added Bento errors.
		 <4>	 11/1/94	VL		1179951: Defined BCS internal prefix.
		 <3>	 9/23/94	VL		1182227: Moved Bento Types from
									StdTypes.idl.
		 <2>	 8/24/94	CG		#1181622: Updated ownership info.
		 <1>	 8/17/94	CG		first checked in
		 <4>	 2/11/94	CG		#114303: Moved 'kNMAPIDxx' consts to
									Bento.r.
		 <3>	 1/21/94	CG		New constant names.
		 <2>	 1/18/94	CG		Fixed resources id nos.
		 <1>	 12/6/93	CG		first checked in

	To Do:
*/

#ifndef _BENTODEF_
#define _BENTODEF_

// Kind
#define kODContainerBento 			"OpenDoc:Apple:Bento"

// Class IDs

#define kODBentoContainerID 		"ODBentoContainer"
#define kODFileContainerID 			"ODFileContainer"
#define kODMemoryContainerID 		"ODMemContainer"
#define kODEmbeddedContainerID 		"ODEmbeddedContainer"
#define kODUpdateContainerID		"ODUpdateContainer"

// Constants
#define kODBentoEmbeddedContainer	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:ContainerType:Embedded"
#define kODBentoFileContainer		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:ContainerType:File"
#define kODBentoMemoryContainer		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:ContainerType:Memory"
#define kODBentoUpdateContainer		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:ContainerType:Update"

#define kODPropVersionList      	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Property:DraftList"
#define kODPropStorageUnitName    	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Metadata:StorageUnitName"
#define kODPropCurrentSURef      	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Metadata:CurrentSURef"
#define kODPropDocumentName      	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Property:DocumentName"

#define kNMAPid1 128

#define kcfrgID				0

#define kODBentoPrefix	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:"
#define kODValue		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:Bytes"
#define kODFrame		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:Frame"
#define kODPart 		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:Part"
#define kODLink 		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:Link"
#define kODLinkSource	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:LinkSource"

// Error codes
// Consult ErrorDef.idl before adding any more errors, to check for uniqueness!

// Unused: -29804

#define	kODErrBentoErr 						-29809
#define kODErrInvalidVersion				-29808
#define kODErrCannotCreateDraftVersion		-29807
#define kODErrVersionListUnavailable		-29806
#define kODErrSemaphoreReleased				-29805
#define kODErrInvalidIDList					-29803
#define kODErrBentoNoSession				-29802
#define kODErrBentoInvalidProperty			-29801
#define kODErrBentoInvalidType				-29800
#define kODErrBentoCannotNewObject			-29799
#define kODErrBentoCannotNewValue			-29798
#define kODErrBentoCannotCreateContainer	-29797
#define kODErrBentoCannotOpenContainer		-29796
#define kODErrBentoInvalidObject			-29795
#define kODErrDraftExistsWithDifferentID	-29794
#define kODErrNoVersionList					-29793
#define kODErrNotPromise					-29792
#define kODErrInvalidZone					-29791
#define kODErrCannotGetDraftVersion			-29790
#define kODErrNoDocumentProperties			-29789

#endif //_BENTODEF_

