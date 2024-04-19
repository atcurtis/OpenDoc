/* @(#)Z 1.6 os2/src/imaging/realshps/CompShpe.h, odimaging, od96os2, odos29646d 96/11/15 15:49:04 (96/08/23 01:42:18) */
/*====START_GENERATED_PROLOG======================================
 */
/*
 *   COMPONENT_NAME: odimaging
 *
 *   CLASSES:   CompositeShape
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
  File:    CompShpe.h

  Contains:  CompositeShape class, private to ODShape.

  Written by:  Chuck Dumont

*/

#ifndef _COMPSHPE_
#define _COMPSHPE_

#ifndef _ODTYPES_
#include "ODTypes.h"
#endif

#ifndef _PLFMDEF_
#include "PlfmDef.h"
#endif

#ifndef _REALSHPE_
#include "RealShpe.h"    // Base class
#endif

//==============================================================================
// CompositeShape
//==============================================================================

ODGeometryMode     GetMode( RealShape*, RealShape* );


class CompositeShape :public RealShape {
  public:

    CompositeShape( RealShape* shap1, RealShape* shap2, ODShapeOp oper );
    ~CompositeShape( );

    ODVMethod ODSize Purge(ODSize size);    // Can't purge rgn!

    ODVMethod void      GetBoundingBox( Environment*, ODRect *bounds );
    ODVMethod void      GetPesimisticBoundingBox( Environment*, ODRect *bounds );
    ODVMethod RealShape*  SetRectangle( const ODRect *rect );
    ODVMethod void      CopyPolygon( Environment*, ODPolygon& );
    ODVMethod RealShape*  SetPolygon( const ODPolygon& );

    ODVMethod ODBoolean  IsSameAs(Environment*, RealShape* compareShape);
    ODVMethod ODBoolean  IsEmpty( Environment* );
    ODVMethod ODBoolean  IsObviousEmpty( Environment* );
    ODVMethod ODBoolean  ContainsPoint(Environment*, ODPoint point);
    ODVMethod ODBoolean  IsRectangular( Environment* );
    ODVMethod ODBoolean  IsObviousRectangular( Environment* );
    ODVMethod ODBoolean  HasGeometry( );

    ODVMethod RealShape*  Copy( Environment* );

    ODVMethod RealShape*  Transform(Environment*, ODTransform* transform);
    ODVMethod RealShape*  Outset(Environment*, ODCoordinate distance);
    ODVMethod RealShape*  Subtract(Environment*, RealShape* diffShape);
    ODVMethod RealShape*  Intersect(Environment*, RealShape* sectShape);
    ODVMethod RealShape*  Union(Environment*, RealShape* sectShape);

//  ODBoolean HasRectangles( Environment* );
//  const ODRectangles& GetRectangles( Environment* );

  protected:

//  void InitRectangles( Environment* );

    void InitQDRegion(Environment*);
    void InitQDPoly(Environment*);
    CompositeShape(  RealShape* shap1, RealShape* shap2, ODShapeOp oper,
                                    ODPolygon fpoly, ODRgnHandle fQDReg);

    ODVMethod ODPlatformShape GetPlatformShape(Environment*, ODGraphicsSystem);

    virtual RealShape*    Clear( );

    RealShape* shape1;
    RealShape* shape2;
    ODShapeOp op;
    ODPolygon        fPolygon;      // Cached equivalent polygon
    ODTransform* fPolygonTransform;
    ODTransform* fRgnTransform;
    ODRect bbox;
    ODBoolean bboxValid;
};


#endif /*_COMPSHPE_*/






