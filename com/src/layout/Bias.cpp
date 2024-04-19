/* @(#)Z 1.6 com/src/layout/Bias.cpp, odlayout, od96os2, odos29646d 96/11/15 15:25:16 (96/07/15 18:31:46) */
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
/*  IBM Change History (most recent first):                         */
/*   20470 03/19/96 tmw/SA Apple DR4 and common Windows/OS2         */
/********************************************************************/
/*
   File:       Bias.cpp

   Contains:   Implementation of private utility functions

   Owned by:   Joshua Susser

   Copyright:  © 1994 - 1995 by Apple Computer, Inc., all rights reserved.

   Change History (most recent first):

        <5>     9/13/95    DM      1277216 GM:API return no ODPoints nor
                                   ODPolygons
        <4>     6/23/95    JBS     1251627 fix refcounting throughout file
        <3>     5/26/95    RR      #1251403: Multithreading naming support
        <2>     9/29/94    JBS     1188214: more coord bias impl
        <1>     9/22/94    JBS     first checked in
*/

/*
These functions are private utilities for use by Layout classes which need
to bias and unbias geometry.  Frames and facets store geometry (shape and
transform objects) in platform-normal coordinates.  They must therefore unbias
geometry in "setter" methods, and bias it in "getter" methods.  These functions
do the necessary transformations.  They assume that geometry objects are
consumed on set, and copied out on get.
*/

#ifndef _BIAS_
#include "Bias.h"
#endif

#ifndef SOM_ODCanvas_xh
#include <Canvas.xh>
#endif

#ifndef SOM_ODShape_xh
#include <Shape.xh>
#endif

#ifndef SOM_ODTransform_xh
#include <Trnsform.xh>
#endif

#ifndef _TEMPOBJ_
#include "TempObj.h"
#endif

#ifdef _FACET_DEBUG_
#include <qprintf.h>
#endif
//==============================================================================
// Functions
//==============================================================================

//------------------------------------------------------------------------------
// BiasPoint
//------------------------------------------------------------------------------

ODPoint BiasPointGet(Environment *ev, ODPoint* point, ODCanvas* biasCanvas)
{
   ODPoint returnPoint = *point;
    TempODTransform biasTransform = kODNULL;

#ifdef _FACET_DEBUG_
    qprintf("Executing BiasPointGet()\n");
#endif
   if ( biasCanvas )
       biasTransform = biasCanvas->AcquireBiasTransform(ev);
   if ( biasTransform )
       biasTransform->TransformPoint(ev, &returnPoint);

   return returnPoint;
}

ODPoint BiasPointSet(Environment *ev, ODPoint* point, ODCanvas* biasCanvas)
{
   ODPoint returnPoint = *point;
    TempODTransform biasTransform = kODNULL;

#ifdef _FACET_DEBUG_
    qprintf("Executing BiasPointSet()\n");
#endif

   if ( biasCanvas )
       biasTransform = biasCanvas->AcquireBiasTransform(ev);
   if ( biasTransform )
       biasTransform->InvertPoint(ev, &returnPoint);

   return returnPoint;
}

//------------------------------------------------------------------------------
// BiasShape
//------------------------------------------------------------------------------

ODShape* BiasShapeGet(Environment *ev, ODShape* shape, ODCanvas* biasCanvas)
{
   ODShape* returnShape = kODNULL;
    TempODTransform biasTransform = kODNULL;

#ifdef _FACET_DEBUG_
    qprintf("Executing BiasShapeGet()\n");
#endif
   if ( shape )
   {
       if ( biasCanvas )
           biasTransform = biasCanvas->AcquireBiasTransform(ev);
       if ( biasTransform )
       {
           returnShape = shape->Copy(ev);
           returnShape->Transform(ev, biasTransform);
       }
       else
       {
           returnShape = shape;
           returnShape->Acquire(ev);
       }
   }
   return returnShape;
}

ODShape* BiasShapeSet(Environment *ev, ODShape* shape, ODCanvas* biasCanvas)
{
    TempODTransform biasTransform = kODNULL;

#ifdef _FACET_DEBUG_
    qprintf("Executing BiasShapeSet()\n");
#endif
   if ( shape )
   {
       if ( biasCanvas )
           biasTransform = biasCanvas->AcquireBiasTransform(ev);
       if ( biasTransform )
           shape->InverseTransform(ev, biasTransform);
   }
   return shape;
}

//------------------------------------------------------------------------------
// BiasTransform
//------------------------------------------------------------------------------

ODTransform* BiasTransformGet(Environment *ev, ODTransform* transform, ODCanvas* biasCanvas)
{
   ODTransform* returnTransform = kODNULL;
    TempODTransform biasTransform = kODNULL;

#ifdef _FACET_DEBUG_
    qprintf("Executing BiasTransformGet()\n");
#endif
   if ( transform )
   {
       if ( biasCanvas )
           biasTransform = biasCanvas->AcquireBiasTransform(ev);
       if ( biasTransform )
       {
           returnTransform = transform->Copy(ev);
           returnTransform->PreCompose(ev, biasTransform);
       }
       else
       {
           returnTransform = transform;
           returnTransform->Acquire(ev);
       }
   }
   return returnTransform;
}

ODTransform* BiasTransformSet(Environment *ev, ODTransform* transform, ODCanvas* biasCanvas)
{
    TempODTransform biasTransform = kODNULL;

#ifdef _FACET_DEBUG_
    qprintf("Executing BiasTransformSet()\n");
#endif
   if ( transform )
   {
       if ( biasCanvas )
           biasTransform = biasCanvas->AcquireBiasTransform(ev);
       if ( biasTransform )
       {
           biasTransform->Invert(ev);
           transform->PreCompose(ev, biasTransform);
           biasTransform->Invert(ev);
       }
   }
   return transform;
}
