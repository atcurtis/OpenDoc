/* @(#)Z 1.4 com/src/layout/Bias.h, odlayout, od96os2, odos29646d 96/11/15 15:29:18 (96/07/15 18:31:51) */
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
   File:       Bias.h

   Contains:   Definitions of private utility functions

   Owned by:   Joshua Susser

   Copyright:  © 1994 by Apple Computer, Inc., all rights reserved.

   Change History (most recent first):

        <2>     9/29/94    JBS     1188214: more coord bias impl
        <1>     9/22/94    JBS     first checked in
*/

#ifndef _BIAS_
#define _BIAS_

#if (defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)
#ifndef _ALTPOINT_
#include "altpoint.h"
#endif
#endif //(defined _PLATFORM_OS2_ || defined _PLATFORM_WIN32_)

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

//==============================================================================
// Classes used by this interface
//==============================================================================

class Environment;
class ODCanvas;
class ODShape;
class ODTransform;

//==============================================================================
// Functions
//==============================================================================

ODPoint
BiasPointGet(Environment *ev, ODPoint* point, ODCanvas* biasCanvas);

ODPoint
BiasPointSet(Environment *ev, ODPoint* point, ODCanvas* biasCanvas);

ODShape*
BiasShapeGet(Environment *ev, ODShape* shape, ODCanvas* biasCanvas);

ODShape*
BiasShapeSet(Environment *ev, ODShape* shape, ODCanvas* biasCanvas);

ODTransform*
BiasTransformGet(Environment *ev, ODTransform* transform, ODCanvas* biasCanvas);

ODTransform*
BiasTransformSet(Environment *ev, ODTransform* transform, ODCanvas* biasCanvas);

#endif // _BIAS_
