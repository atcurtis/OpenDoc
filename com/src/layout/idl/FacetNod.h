/* @(#)Z 1.3 com/src/layout/FacetNod.h, odlayout, od96os2, odos29646d 96/11/15 15:29:19 (96/07/15 18:32:17) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odlayout
 *
 *   CLASSES:   FacetNode
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
   File:       FacetNod.h

   Contains:   Definition of class FacetNode

   Written by: Joshua Susser

   Copyright:  © 1992-94 by Apple Computer, Inc., all rights reserved.

   Change History (most recent first):

        <1>     6/29/94    JBS     first checked in
        <0>     6/29/94    SV      SOMverted
        <2>     3/15/94    MB      Changes to support SCpp/ASLM builds,
                                   #1150864.
        <1>     3/11/94    JBS     first checked in
        <3>      2/3/94    JA      Tiger Team Makeover!
        <2>    11/12/93    JBS     use Facets
        <1>     9/23/93    JBS     first checked in
*/

#ifndef _FACETNOD_
#define _FACETNOD_

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _ODTYPES_
#include <ODTypes.h>
#endif

#ifndef _NODE_
#include "Node.h"
#endif

//==============================================================================
// Theory of Operation
//==============================================================================

/*

FacetNode-s stitch Facet-s together into a tree structure.  Each Facet has a
FacetNode which holds its place in the visible frame structure for the document.

*/

//=====================================================================================
// Classes defined in this interface
//=====================================================================================

class FacetNode;

//=====================================================================================
// Classes used by this interface
//=====================================================================================

class Node;
class ODFacet;

//=====================================================================================
// FacetNode
//=====================================================================================

class FacetNode : public Node
{

public:

   FacetNode(ODFacet* facet);

   ODVMethod ~  FacetNode();

   ODVMethod ODFacet* GetFacet();

private:

   ODFacet* fFacet;
};

#endif // _FACETNOD_
