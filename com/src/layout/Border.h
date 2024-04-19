/* @(#)Z 1.3 com/src/layout/Border.h, odlayout, od96os2, odos29646d 96/11/15 15:29:19 (96/07/15 18:31:58) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odlayout
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
   File:       Border.h

   Contains:   Definitions of active border display routines

   Owned by:   Joshua Susser

   Copyright:  © 1995 by Apple Computer, Inc., all rights reserved.

   Change History (most recent first):

        <1>     2/28/95    JBS     first checked in
        <0>     6/30/94    JBS     1198509 created
*/

#ifndef _BORDER_
#define _BORDER_

//==============================================================================
// Classes used by this interface
//==============================================================================

class Environment;
class ODFacet;
class ODShape;

//==============================================================================
// Functions
//==============================================================================

void
ODDrawBorder(Environment *ev, ODShape* borderShape, ODFacet* facet);

void
ODInvalidateBorder(Environment *ev, ODShape* borderShape, ODFacet* facet);

#endif // _BORDER_
