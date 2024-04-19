/* @(#)Z 1.4 com/src/core/ToknTabl.h, odcore, od96os2, odos29646d 96/11/15 15:29:16 (96/10/29 09:21:50) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odcore
 *
 *   CLASSES:   TokenTable
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
	File:		ToknTabl.h

	Contains:	The TokenTable class.

	Owned by:	Nick Pilch

	Copyright:	© 1993 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		 <4>	 8/19/94	NP		1181622: Ownership fix.
		 <3>	 7/29/94	CG		NameSpace API changes.
		 <2>	 6/27/94	CG		Removed fEv - not thread safe.
		 <1>	  6/7/94	CG		first checked in
		 <3>	11/15/93	NP		Changed ODSymbols to ODNameSpaceManager.
		 <2>	10/28/93	NP		GetType returns a boolean instead of
									throwing an exception.
		 <1>	10/25/93	NP		first checked in
		 <4>	  8/9/93	NP		Added class id string.
		 <3>	 7/21/93	NP		Added a virtual keyword (temporary for
									ASLM).
		 <2>	 6/23/93	RCR		Moved ODTypeToken to ODTypes.h
		 <1>	 6/22/93	NP		first checked in

	To Do:
*/

#ifndef _ODTOKNTABL_
#define _ODTOKNTABL_

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

//==============================================================================
// Theory of Operation
//==============================================================================

/*
	This class keeps a table of ODTypes and their corresponding unique tokens.
*/

//==============================================================================
// Scalar Types
//==============================================================================


//==============================================================================
// Classes defined in this interface
//==============================================================================

class TokenTable;

//==============================================================================
// Classes used by this interface
//==============================================================================

class ODValueNameSpace;
class ODNameSpaceManager;

//==============================================================================
// ODClass
//==============================================================================

#define kODTokenTableID "appl:TokenTable$class,1.0.0"

class TokenTable
{
  public:

	TokenTable(ODNameSpaceManager* mgr);
	ODVMethod void Initialize();
	virtual ~TokenTable();

	ODVMethod ODTypeToken	Tokenize(ODType type);
		
		// Returns a unique token for type. If the type was previously
		//	tokenized, the original token is returned. Throws kODErrOutOfMemory
		//	if tokenization fails.
	
	ODVMethod void			RemoveEntry(ODType type);
		
		// Removes type from the table. If type was not
		//	previously tokenized, no error is signalled.
	
	ODVMethod ODBoolean	GetType(ODTypeToken token, ODType* type);
		
		// Returns type, given token. If token does not exist in the table,
		//	kODFalse is returned. Otherwise, kODTrue is returned.
	
  protected:

	ODNameSpaceManager*	fNameSpaceManager;
	ODValueNameSpace*	fNameSpace;
	ODULong				fNextUniqueID;
};


#endif // _ODTOKNTABL_
