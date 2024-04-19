/* @(#)Z 1.4 com/src/layout/FacetNod.cpp, odlayout, od96os2, odos29646d 96/11/15 15:25:23 (96/07/15 18:32:13) */
//====START_GENERATED_PROLOG======================================
//
//
//   COMPONENT_NAME: odlayout
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
/*
   File:       FacetNod.cpp

   Contains:   Implementation of class FacetNode

   Owned by:   Joshua Susser

   Copyright:  © 1993 - 1995 by Apple Computer, Inc., all rights reserved.

   Change History (most recent first):

        <2>     5/26/95    RR      #1251403: Multithreading naming support
        <1>     6/29/94    JBS     first checked in
        <0>     6/29/94    SV      SOMverted
        <1>     3/11/94    JBS     first checked in
        <2>    11/12/93    JBS     use Facets
        <1>     9/23/93    JBS     first checked in
*/

#ifndef _FACETNOD_
#include "FacetNod.h"
#endif

#ifdef _PLATFORM_MACINTOSH_
#pragma segment ODFacet
#endif

#ifdef _FACET_DEBUG_
#include <qprintf.h>
#endif
//=====================================================================================
// Class FacetNode
//=====================================================================================

//------------------------------------------------------------------------------
// FacetNode::FacetNode
//------------------------------------------------------------------------------

FacetNode::FacetNode(ODFacet* facet) : Node()
{
#ifdef _FACET_DEBUG_
    qprintf("Executing FacetNode::FacetNode()\n");
#endif
   fFacet = facet;
}

//------------------------------------------------------------------------------
// FacetNode::~FacetNode
//------------------------------------------------------------------------------

FacetNode::~FacetNode()
{

#ifdef _FACET_DEBUG_
    qprintf("Executing FacetNode::~FacetNode()\n");
#endif
}

//------------------------------------------------------------------------------
// FacetNode::AcquireFrame
//------------------------------------------------------------------------------

ODFacet* FacetNode::GetFacet()
{
#ifdef _FACET_DEBUG_
    qprintf("Executing FacetNode::GetFacet()\n");
#endif
   return fFacet;
}

